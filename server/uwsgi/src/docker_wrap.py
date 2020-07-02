import io
import tarfile
import docker
import types
import os
import time
import random
user_and_group = '13467:13468'


class MyPythonContainer:

    def __init__(self):
        client = docker.DockerClient(base_url='unix://var/run/docker.sock')
        client.containers.prune()
        self.container = client.containers.run(
            'python',
            'tail -f /dev/null',  # keep the container open doing nothing
            cap_drop=['ALL'],
            network_mode='none',
            read_only=False,
            security_opt=['no-new-privileges'],
            # auto_remove=True,
            detach=True,
            user=user_and_group
        )

    def pause(self):
        self.container.pause()

    @staticmethod
    def make_tar(name, string):
        file_like_object = io.BytesIO()
        with tarfile.open(fileobj=file_like_object, mode='w') as tar:
            b = string.encode()
            t = tarfile.TarInfo(name)
            t.size = len(b)
            tar.addfile(t, io.BytesIO(b))

        data = file_like_object.getvalue()
        return data

    def upload(self, target, data):
        dirname, basename = os.path.split(target)
        tar_data = MyPythonContainer.make_tar(basename, data)
        self.container.put_archive(dirname, tar_data)

    def download(self, target):
        dirname, basename = os.path.split(target)
        bits, stats = self.container.get_archive(target)

        file_like_object = io.BytesIO()
        for chunk in bits:
            file_like_object.write(chunk)
        file_like_object.seek(0)

        with tarfile.open(fileobj=file_like_object, mode='r') as tar:
            return tar.extractfile(basename).read()

    def exec(self, command):
        SCRIPT_PATH = '/tmp/script.sh'
        script_format = '\n'.join([
            '#! /bin/bash',
            'touch /tmp/stdout',
            'touch /tmp/stderr',
            '/bin/bash -c "{} >/tmp/stdout 2>/tmp/stderr"',
            'echo $? > /tmp/exit_code'
        ])
        script = script_format.format(command)
        self.upload(SCRIPT_PATH, script)

        self.container.exec_run(
            ['bash', SCRIPT_PATH],
            stdout=False,
            stderr=False,
            user=user_and_group,
            detach=True,
            demux=True
        )

    def exec_result(self):
        stdout = stderr = exit_code = None
        try:
            exit_code = self.download('/tmp/exit_code').hex()
            stdout = self.download('/tmp/stdout').hex()
            stderr = self.download('/tmp/stderr').hex()
        except docker.errors.NotFound:
            return None

        return {
            'exit_code': exit_code,
            'stdout': stdout,
            'stderr': stderr,
        }

    def cleanup(self):
        self.container.kill()
        self.container.remove()

    def __enter__(self):
        return self

    def __exit__(self, type, value, traceback):
        self.cleanup()


def run_code(code_as_string, timeout):
    with MyPythonContainer() as c:
        c.upload('/tmp/usercode.py', code_as_string)
        c.exec('python /tmp/usercode.py')
        for i in range(timeout):
            time.sleep(1)
            result = c.exec_result()
            if result is not None:
                return result

        c.pause()
        raise TimeoutError('Script timed out')


if __name__ == '__main__':
    result = run_code(
        "print(5); import sys; print('fatal error', file=sys.stderr)", 10)
    print(result)

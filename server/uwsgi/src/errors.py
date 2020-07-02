import functools
from http import HTTPStatus
import traceback

class ServiceError(Exception):
   def __init__(self, msg, status):
       self.msg = msg
       self.status = status

class BadRequest(ServiceError):
   def __init__(self):
       super().__init__('Bad request', HTTPStatus.BAD_REQUEST)


class Unauthorised(ServiceError):
   def __init__(self):
       super().__init__('Unauthorised', HTTPStatus.UNAUTHORIZED)


class Created(ServiceError):
   def __init__(self):
       super().__init__('User account created. You will receive confirmation email.', HTTPStatus.CREATED)


class UserKeyError(BadRequest):
    pass


class DictWrapper:
     def __init__(self, data):
         self.data = data

     def __getitem__(self, key):
         try:
             return self.data[key]
         except KeyError:
             raise UserKeyError()


def catch_errors(f):
    @functools.wraps(f)
    def inner(*args,**kwargs):
        try:
            return f(*args,**kwargs), HTTPStatus.OK
        except ServiceError as e:
            return e.msg, e.status
        except Exception:
            traceback.print_exc()
            return 'Server error', HTTPStatus.INTERNAL_SERVER_ERROR
    return inner
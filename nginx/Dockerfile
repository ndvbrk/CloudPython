FROM nginx

USER root

RUN rm -f /etc/nginx/conf.d/default.conf
RUN chown nginx:nginx /var/cache/nginx
RUN chown nginx:nginx  /var/log/nginx
RUN touch /var/run/nginx.pid
RUN chown nginx:nginx  /var/run/nginx.pid
USER nginx


COPY --chown=nginx nginx.conf /etc/nginx/conf.d/
COPY --chown=nginx certificate.crt /etc/nginx/cert.crt
COPY --chown=nginx privatekey.key /etc/nginx/cert.key

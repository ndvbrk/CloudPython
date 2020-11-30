FROM nginx

USER root

RUN rm -f /etc/nginx/conf.d/default.conf
RUN chown nginx:nginx /var/cache/nginx
RUN chown nginx:nginx  /var/log/nginx
RUN touch /var/run/nginx.pid
RUN chown nginx:nginx  /var/run/nginx.pid
USER nginx

COPY nginx.conf /etc/nginx/conf.d/
COPY certificate.crt /etc/nginx/cert.crt
COPY privatekey.key /etc/nginx/cert.key
RUN chown nginx  /etc/nginx/conf.d/
RUN chown nginx  /etc/nginx/cert.crt
RUN chown nginx  /etc/nginx/cert.key
FROM node:20-alpine
WORKDIR /app
COPY package.json .
RUN npm install
COPY server.js .
COPY public/ public/
EXPOSE 3002
CMD ["node", "server.js"]

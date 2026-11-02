FROM drogonframework/drogon:latest

WORKDIR /app

# Копируем все исходники внутрь контейнера
COPY . /app

# Собираем проект
RUN cmake . && make

# Открываем порт
EXPOSE 8080

# Команда при старте контейнера
CMD ["./bar_backend"]
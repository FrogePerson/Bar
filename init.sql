CREATE TABLE IF NOT EXISTS dishes (
    id SERIAL PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    cost INTEGER NOT NULL,
    image_url VARCHAR(512) DEFAULT '/img/dish-placeholder.png',
    description TEXT DEFAULT '',
    recommended BOOLEAN DEFAULT FALSE,
    category VARCHAR(255) DEFAULT 'all'
);


INSERT INTO dishes (name, cost, image_url, description, category, recommended) VALUES ('Бургер0', 350, '/img/dish-placeholder.png', 'Описание 0', 'kitchen', 'TRUE');
INSERT INTO dishes (name, cost, image_url, description, category, recommended) VALUES ('Бургер1', 650, '/img/i.png', 'Описание 1', 'cocktails', 'TRUE');
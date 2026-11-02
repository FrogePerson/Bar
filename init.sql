CREATE TABLE IF NOT EXISTS dishes (
    id SERIAL PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    cost INTEGER NOT NULL
);

INSERT INTO dishes (name, cost) VALUES ('Бургер с жабой', 350);
INSERT INTO dishes (name, cost) VALUES ('Абсент', 650);
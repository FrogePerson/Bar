let allDishes = [];

document.addEventListener('DOMContentLoaded', () => {
    // Ставим минимальную дату бронирования на сегодня
    const dateInput = document.getElementById('bookingDate');
    if (dateInput) {
        dateInput.min = new Date().toISOString().split('T')[0];
    }

    fetchEvents();
    fetchMenu();
    fetchGallery();
});

// Защита от XSS при вставке данных из БД
function escapeHtml(str) {
    if (!str) return '';
    return String(str)
        .replace(/&/g, '&amp;')
        .replace(/</g, '&lt;')
        .replace(/>/g, '&gt;')
        .replace(/"/g, '&quot;')
        .replace(/'/g, '&#039;');
}

// 1. Получение меню
async function fetchMenu() {
    // ДОБАВЛЕНО: Проверяем, загружали ли мы уже меню
    if (allDishes.length > 0) {
        return; // Выходим из функции, если данные уже есть
    }

    const container = document.getElementById('menu-container');
    try {
        const response = await fetch('/api/menu');
        if (!response.ok) throw new Error(`HTTP error: ${response.status}`);
        allDishes = await response.json();
        renderMenu(allDishes);
    } catch (err) {
        console.warn('Сервер /api/menu недоступен:', err);
        container.innerHTML = '<p class="loading">Меню обновляется. Уточняйте позиции у бармена.</p>';
    }
}

function renderMenu(dishes) {
    const container = document.getElementById('menu-container');
    if (!dishes || dishes.length === 0) {
        container.innerHTML = '<p class="loading">В данной категории пока ничего нет.</p>';
        return;
    }

    container.innerHTML = dishes.map(dish => `
        <div class="menu-card">
            <img class="menu-img" 
                 src="${escapeHtml(dish.imageUrl) || 'img/dish-placeholder.png'}" 
                 alt="${escapeHtml(dish.name)}" 
                 loading="lazy"
                 onerror="this.onerror=null; this.src='img/dish-placeholder.png';">
            <div class="menu-info">
                <div class="menu-header-row">
                    <span class="dish-name">${escapeHtml(dish.name)}</span>
                    <span class="dish-cost">${Number(dish.cost) || 0} ₽</span>
                </div>
                <p class="dish-desc">${escapeHtml(dish.description)}</p>
            </div>
        </div>
    `).join('');
}

function filterMenu(category, event) {
    document.querySelectorAll('.tab-btn').forEach(btn => btn.classList.remove('active'));
    if (event && event.target) {
        event.target.classList.add('active');
    }

    if (category === 'all') {
        renderMenu(allDishes);
    } else {
        const filtered = allDishes.filter(d => d.category === category);
        renderMenu(filtered);
    }
}

// 2. Получение афиши
async function fetchEvents() {
    const container = document.getElementById('events-container');
    try {
        const response = await fetch('/api/events');
        if (!response.ok) throw new Error(`HTTP error: ${response.status}`);
        const events = await response.json();

        if (!events || events.length === 0) {
            container.innerHTML = '<p class="loading">На ближайшие дни мероприятий не запланировано.</p>';
            return;
        }

        container.innerHTML = events.map(evt => `
            <div class="event-card">
                <img class="event-img" 
                     src="${escapeHtml(evt.imageUrl) || 'img/event-placeholder.jpg'}" 
                     alt="${escapeHtml(evt.title)}" 
                     loading="lazy"
                     onerror="this.src='img/event-placeholder.jpg'">
                <div class="event-info">
                    <span class="event-date">${escapeHtml(evt.date)}</span>
                    <h3>${escapeHtml(evt.title)}</h3>
                    <p class="dish-desc">${escapeHtml(evt.description)}</p>
                </div>
            </div>
        `).join('');
    } catch (err) {
        console.warn('Сервер /api/events недоступен:', err);
        container.innerHTML = '<p class="loading">Афиша событий временно недоступна.</p>';
    }
}

// 3. Получение галереи
async function fetchGallery() {
    const container = document.getElementById('gallery-container');
    try {
        const response = await fetch('/api/gallery');
        if (!response.ok) throw new Error(`HTTP error: ${response.status}`);
        const photos = await response.json();

        if (!photos || photos.length === 0) {
            container.innerHTML = '<p class="loading">Фотографии обновляются.</p>';
            return;
        }

        container.innerHTML = photos.map(photo => `
            <div class="gallery-item">
                <img src="${escapeHtml(photo.imageUrl)}" 
                     alt="${escapeHtml(photo.caption || 'Интерьер бара')}" 
                     loading="lazy">
            </div>
        `).join('');
    } catch (err) {
        console.warn('Сервер /api/gallery недоступен:', err);
        container.innerHTML = '<p class="loading">Галерея временно недоступна.</p>';
    }
}

// 4. Модальное окно бронирования
function openBookingModal() {
    const modal = document.getElementById('bookingModal');
    modal.style.display = 'flex';
    modal.setAttribute('aria-hidden', 'false');
}

function closeBookingModal() {
    const modal = document.getElementById('bookingModal');
    modal.style.display = 'none';
    modal.setAttribute('aria-hidden', 'true');
}

window.onclick = function(event) {
    const modal = document.getElementById('bookingModal');
    if (event.target === modal) closeBookingModal();
};

async function submitBooking(event) {
    event.preventDefault();
    const btn = document.getElementById('bookingSubmitBtn');
    btn.disabled = true;
    btn.innerText = 'Отправка...';

    const formData = new FormData(event.target);
    const payload = Object.fromEntries(formData.entries());

    try {
        const response = await fetch('/api/book', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(payload)
        });

        if (response.ok) {
            alert('Столик успешно забронирован! Мы свяжемся с вами для подтверждения.');
            closeBookingModal();
            event.target.reset();
        } else {
            alert('Не удалось оформить бронь. Пожалуйста, позвоните нам напрямую.');
        }
    } catch (e) {
        console.error(e);
        alert('Сервер временно недоступен. Пожалуйста, позвоните по телефону бара.');
    } finally {
        btn.disabled = false;
        btn.innerText = 'Подтвердить заявку';
    }
}
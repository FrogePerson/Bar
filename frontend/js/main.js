document.addEventListener('DOMContentLoaded', () => {
    // Ограничение даты для бронирования (из твоего оригинального кода)
    const dateInput = document.getElementById('bookingDate');
    if (dateInput) {
        dateInput.min = new Date().toISOString().split('T')[0];
    }

    fetchEvents();
    fetchGallery();
    
    // Загружаем категорию 'all' по умолчанию
    fetchMenu('all'); 
});

// Экранирование HTML
function escapeHtml(str) {
    if (!str) return '';
    return String(str)
        .replace(/&/g, '&amp;')
        .replace(/</g, '&lt;')
        .replace(/>/g, '&gt;')
        .replace(/"/g, '&quot;')
        .replace(/'/g, '&#039;');
}

// ---------------------------------------------------
// ЛОГИКА МЕНЮ 
// ---------------------------------------------------

// Запрашиваем меню с бэкенда с передачей категории
async function fetchMenu(category = 'all') {
    const regularContainer = document.getElementById('menu-regular-container') || document.getElementById('menu-container');
    const topContainer = document.getElementById('menu-top-container');

    try {
        // Отправляем GET-запрос с query-параметром (например, /api/menu?category=kitchen)
        const response = await fetch(`/api/menu?category=${encodeURIComponent(category)}`);
        if (!response.ok) throw new Error(`HTTP error: ${response.status}`);
        
        const dishes = await response.json();
        renderMenu(dishes);
    } catch (err) {
        console.warn('Сервер /api/menu недоступен:', err);
        regularContainer.innerHTML = '<p class="loading">Меню обновляется. Уточняйте позиции у бармена.</p>';
        if (topContainer) topContainer.innerHTML = '';
    }
}

// Разделение массива и отрисовка
function renderMenu(dishes) {
    // Если id "menu-regular-container" еще нет в HTML, используем старый "menu-container" для обратной совместимости
    const regularContainer = document.getElementById('menu-regular-container') || document.getElementById('menu-container');
    const topContainer = document.getElementById('menu-top-container');

    if (!dishes || dishes.length === 0) {
        regularContainer.innerHTML = '<p class="loading">В данной категории пока ничего нет.</p>';
        if (topContainer) topContainer.innerHTML = '';
        return;
    }

    // Фильтруем на основе поля recommended (или is_top, смотря как назвали в C++)
    const topDishes = dishes.filter(d => d.recommended === true);
    const regularDishes = dishes.filter(d => d.recommended !== true);

    // Функция-шаблон для генерации одной карточки
    const createCard = (dish) => {
        const imageUrl = (dish.imageUrl || dish.image_url) ? escapeHtml(dish.imageUrl || dish.image_url) : 'img/dish-placeholder.png';
        const name = dish.name ? escapeHtml(dish.name) : 'Без названия';
        const cost = Number(dish.cost) || 0;
        const description = dish.description ? escapeHtml(dish.description) : '';

        return `
            <div class="menu-card">
                <img class="menu-img" 
                     src="${imageUrl}" 
                     alt="${name}" 
                     loading="lazy"
                     onerror="this.onerror=null; this.style.visibility='hidden';">
                <div class="menu-info">
                    <div class="menu-header-row">
                        <span class="dish-name">${name}</span>
                        <span class="dish-cost">${cost} ₽</span>
                    </div>
                    ${description ? `<p class="dish-desc">${description}</p>` : ''}
                </div>
            </div>
        `;
    };

    // Отрисовываем
    if (regularContainer) {
    regularContainer.innerHTML = dishes.map(createCard).join('');
    }
}

// Вызывается при клике на вкладки меню (из HTML: onclick="filterMenu('kitchen', event)")
function filterMenu(category, event) {
    // Обновляем визуальное состояние кнопок
    document.querySelectorAll('.tab-btn').forEach(btn => btn.classList.remove('active'));
    if (event && event.target) {
        event.target.classList.add('active');
    }

    // Идем на сервер за отфильтрованным списком
    fetchMenu(category);
}


// ---------------------------------------------------
// АФИША 
// ---------------------------------------------------
async function fetchEvents() {
    const container = document.getElementById('events-container');
    if (!container) return;

    try {
        const response = await fetch('/api/events');
        if (!response.ok) throw new Error(`HTTP error: ${response.status}`);
        const events = await response.json();

        if (!Array.isArray(events) || events.length === 0) {
            container.innerHTML = '<p class="loading">На ближайшие дни мероприятий не запланировано.</p>';
            return;
        }

        // Рендерим большие слайды-баннеры
        container.innerHTML = events.map(evt => `
            <a href="/event.html?id=${evt.id}" class="event-banner-slide">
                <img class="event-banner-img" 
                     src="${escapeHtml(evt.imageUrl || evt.image_url) || ''}" 
                     alt="${escapeHtml(evt.name)}" 
                     loading="lazy"
                     onerror="this.style.display='none'">
                <div class="event-banner-overlay"></div>
                <div class="event-banner-content">
                    <span class="event-banner-date">${escapeHtml(evt.event_date || evt.formatted_date)}</span>
                    <h3 class="event-banner-title">${escapeHtml(evt.name)}</h3>
                    <p class="event-banner-desc">${escapeHtml(evt.description)}</p>
                </div>
            </a>
        `).join('');
    } catch (err) {
        console.warn('Сервер /api/events недоступен:', err);
        container.innerHTML = '<p class="loading">Афиша событий временно недоступна.</p>';
    }
}


// ---------------------------------------------------
// ЛОГИКА СТРЕЛОК ДЛЯ АФИШИ
// ---------------------------------------------------
document.addEventListener('DOMContentLoaded', () => {
    const container = document.getElementById('events-container');
    const prevBtn = document.getElementById('events-prev');
    const nextBtn = document.getElementById('events-next');

    if (container && prevBtn && nextBtn) {
        // Прокрутка влево
        prevBtn.addEventListener('click', () => {
            // Прокручиваем на 90% ширины контейнера (ширина одного слайда)
            container.scrollBy({ left: -container.clientWidth * 0.9, behavior: 'smooth' });
        });

        // Прокрутка вправо
        nextBtn.addEventListener('click', () => {
            container.scrollBy({ left: container.clientWidth * 0.9, behavior: 'smooth' });
        });
    }
});



// ---------------------------------------------------
// ГАЛЕРЕЯ 
// ---------------------------------------------------
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

// ---------------------------------------------------
// БРОНИРОВАНИЕ 
// ---------------------------------------------------
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
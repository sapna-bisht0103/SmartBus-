// =============================================
//  SMARTBUS — JavaScript
// =============================================

/* ---- Navbar Scroll Effect ---- */
const navbar = document.getElementById('navbar');
window.addEventListener('scroll', () => {
  if (window.scrollY > 50) {
    navbar.classList.add('scrolled');
  } else {
    navbar.classList.remove('scrolled');
  }
});

/* ---- Hamburger Menu ---- */
const hamburger = document.getElementById('hamburger');
const navLinks  = document.getElementById('navLinks');
hamburger.addEventListener('click', () => {
  navLinks.classList.toggle('open');
  hamburger.classList.toggle('active');
});
// Close menu on link click
navLinks.querySelectorAll('a').forEach(link => {
  link.addEventListener('click', () => {
    navLinks.classList.remove('open');
    hamburger.classList.remove('active');
  });
});

/* ---- Smooth Active Nav Link Highlight ---- */
const sections = document.querySelectorAll('section[id]');
const navItems = document.querySelectorAll('.nav-links a');
window.addEventListener('scroll', () => {
  let current = '';
  sections.forEach(sec => {
    if (window.scrollY >= sec.offsetTop - 120) current = sec.getAttribute('id');
  });
  navItems.forEach(a => {
    a.classList.remove('active-link');
    if (a.getAttribute('href') === '#' + current) a.classList.add('active-link');
  });
});

/* ---- Scroll Reveal ---- */
const revealEls = document.querySelectorAll(
  '.feature-card, .module-card, .oop-card, .team-card, .login-card, .run-box, .stat'
);
revealEls.forEach(el => el.classList.add('reveal'));

const revealObserver = new IntersectionObserver((entries) => {
  entries.forEach((entry, i) => {
    if (entry.isIntersecting) {
      setTimeout(() => entry.target.classList.add('visible'), i * 80);
      revealObserver.unobserve(entry.target);
    }
  });
}, { threshold: 0.12 });

revealEls.forEach(el => revealObserver.observe(el));

/* ---- Login Card Toggle ---- */
function showInfo(role) {
  const allDetails = document.querySelectorAll('.login-details');
  const allCards   = document.querySelectorAll('.login-card');

  allDetails.forEach(d => d.classList.remove('show'));
  allCards.forEach(c => c.classList.remove('active'));

  const target = document.getElementById('info-' + role);
  const card   = target.closest('.login-card');

  if (target) target.classList.add('show');
  if (card)   card.classList.add('active');
}

/* ---- Animated Counter (Stats) ---- */
function animateCounter(el, target, duration = 1500) {
  let start = 0;
  const step = target / (duration / 16);
  const timer = setInterval(() => {
    start += step;
    if (start >= target) { el.textContent = target + '+'; clearInterval(timer); }
    else el.textContent = Math.floor(start) + '+';
  }, 16);
}

const statsObserver = new IntersectionObserver((entries) => {
  entries.forEach(entry => {
    if (entry.isIntersecting) {
      const nums = entry.target.querySelectorAll('.stat-num');
      nums.forEach(num => {
        const val = parseInt(num.textContent);
        if (!isNaN(val)) animateCounter(num, val);
      });
      statsObserver.unobserve(entry.target);
    }
  });
}, { threshold: 0.5 });

const heroStats = document.querySelector('.hero-stats');
if (heroStats) statsObserver.observe(heroStats);

/* ---- Bus Card Pulse on Hover ---- */
const busCard = document.querySelector('.bus-card');
if (busCard) {
  busCard.addEventListener('mouseenter', () => {
    busCard.style.transform = 'scale(1.03)';
    busCard.style.transition = '0.3s ease';
  });
  busCard.addEventListener('mouseleave', () => {
    busCard.style.transform = 'scale(1)';
  });
}

/* ---- Scroll to Top Button ---- */
const scrollBtn = document.createElement('button');
scrollBtn.innerHTML = '<i class="fas fa-arrow-up"></i>';
scrollBtn.style.cssText = `
  position: fixed; bottom: 2rem; right: 2rem;
  width: 44px; height: 44px;
  background: linear-gradient(135deg, #4f46e5, #f97316);
  color: white; border: none; border-radius: 50%;
  font-size: 1rem; cursor: pointer;
  box-shadow: 0 4px 20px rgba(79,70,229,0.4);
  opacity: 0; transition: opacity 0.3s, transform 0.3s;
  z-index: 999; display: flex; align-items: center; justify-content: center;
`;
document.body.appendChild(scrollBtn);

window.addEventListener('scroll', () => {
  scrollBtn.style.opacity = window.scrollY > 400 ? '1' : '0';
  scrollBtn.style.transform = window.scrollY > 400 ? 'scale(1)' : 'scale(0.8)';
});
scrollBtn.addEventListener('click', () => window.scrollTo({ top: 0, behavior: 'smooth' }));

// BTT Studio — Practice Mod 1.0 Web Showcase App

document.addEventListener('DOMContentLoaded', () => {
  initDraggableController();
  initColorPickers();
  initPresets();
  initTabs();
  initCopyButtons();
  initButtonClicks();
});

// State
const state = {
  buttonColor: '#ffffff',
  pressedColor: '#e134eb',
  stickColor: '#ffffff',
  ringColor: '#808080',
  bgColor: 'rgba(0, 0, 0, 0.55)',
  isMulticolor: false,
  preset: 'center',
};

// 1. Draggable Controller inside Stage Canvas
function initDraggableController() {
  const controller = document.getElementById('interactiveController');
  const stage = document.getElementById('stageCanvas');
  if (!controller || !stage) return;

  let isDragging = false;
  let startX = 0, startY = 0;
  let initialLeft = 0, initialTop = 0;

  controller.addEventListener('pointerdown', (e) => {
    // If clicking a button, allow button click
    isDragging = true;
    controller.setPointerCapture(e.pointerId);
    startX = e.clientX;
    startY = e.clientY;
    initialLeft = controller.offsetLeft;
    initialTop = controller.offsetTop;
    controller.style.transition = 'none';
  });

  controller.addEventListener('pointermove', (e) => {
    if (!isDragging) return;
    const dx = e.clientX - startX;
    const dy = e.clientY - startY;

    let newX = initialLeft + dx;
    let newY = initialTop + dy;

    // Boundaries
    const maxX = stage.clientWidth - controller.clientWidth;
    const maxY = stage.clientHeight - controller.clientHeight;

    newX = Math.max(10, Math.min(newX, maxX - 10));
    newY = Math.max(10, Math.min(newY, maxY - 10));

    controller.style.left = `${newX}px`;
    controller.style.top = `${newY}px`;

    updateCoordinatesDisplay(newX, newY);
  });

  const stopDrag = (e) => {
    if (isDragging) {
      isDragging = false;
      controller.style.transition = 'transform 0.1s ease';
      try { controller.releasePointerCapture(e.pointerId); } catch(err) {}
    }
  };

  controller.addEventListener('pointerup', stopDrag);
  controller.addEventListener('pointercancel', stopDrag);
}

function updateCoordinatesDisplay(x, y) {
  const coordsEl = document.getElementById('currentCoordinates');
  if (coordsEl) {
    coordsEl.textContent = `X: ${Math.round(x)} | Y: ${Math.round(y)}`;
  }
}

// 2. Position Presets
function initPresets() {
  const stage = document.getElementById('stageCanvas');
  const controller = document.getElementById('interactiveController');
  const presetBtns = document.querySelectorAll('.preset-btn');
  if (!stage || !controller) return;

  presetBtns.forEach(btn => {
    btn.addEventListener('click', () => {
      presetBtns.forEach(b => b.classList.remove('active'));
      btn.classList.add('active');

      const preset = btn.dataset.preset;
      controller.style.transition = 'all 0.35s cubic-bezier(0.2, 0.8, 0.2, 1)';

      const maxX = stage.clientWidth - controller.clientWidth;
      const maxY = stage.clientHeight - controller.clientHeight;

      let targetX = (stage.clientWidth - controller.clientWidth) / 2;
      let targetY = (stage.clientHeight - controller.clientHeight) / 2;

      switch(preset) {
        case 'bottom-left':
          targetX = 25;
          targetY = maxY - 25;
          break;
        case 'bottom-right':
          targetX = maxX - 25;
          targetY = maxY - 25;
          break;
        case 'top-left':
          targetX = 25;
          targetY = 25;
          break;
        case 'top-right':
          targetX = maxX - 25;
          targetY = 25;
          break;
        case 'center':
        default:
          targetX = (stage.clientWidth - controller.clientWidth) / 2;
          targetY = (stage.clientHeight - controller.clientHeight) / 2;
          break;
      }

      controller.style.left = `${targetX}px`;
      controller.style.top = `${targetY}px`;
      updateCoordinatesDisplay(targetX, targetY);
    });
  });
}

// 3. Color Pickers & Multicolor Mode
function initColorPickers() {
  const multicolorBtn = document.getElementById('toggleMulticolorBtn');
  const controllerBg = document.querySelector('.controller-bg');
  const regularBtns = document.querySelectorAll('.ctrl-btn, .ctrl-rect');
  const sticks = document.querySelectorAll('.ctrl-stick');
  const rings = document.querySelectorAll('.ctrl-stick-ring');

  if (multicolorBtn) {
    multicolorBtn.addEventListener('click', () => {
      state.isMulticolor = !state.isMulticolor;

      if (state.isMulticolor) {
        multicolorBtn.style.background = 'linear-gradient(135deg, #ff0055, #00bbff)';
        multicolorBtn.style.color = '#fff';
        multicolorBtn.innerHTML = '✨ Multicolor RGB : ACTIF';
        
        regularBtns.forEach(b => b.classList.add('rainbow-active'));
        sticks.forEach(s => s.classList.add('rainbow-active'));
        rings.forEach(r => r.style.borderColor = '#00c3ff');
        if (controllerBg) controllerBg.classList.add('rainbow-bg-active');
      } else {
        multicolorBtn.style.background = '';
        multicolorBtn.style.color = '';
        multicolorBtn.innerHTML = '✨ Activer Multicolor (RGB Rainbow)';
        
        regularBtns.forEach(b => b.classList.remove('rainbow-active'));
        sticks.forEach(s => s.classList.remove('rainbow-active'));
        rings.forEach(r => r.style.borderColor = state.ringColor);
        if (controllerBg) controllerBg.classList.remove('rainbow-bg-active');
        applyStaticColors();
      }
    });
  }

  // Color option buttons
  document.querySelectorAll('.color-option-btn').forEach(btn => {
    btn.addEventListener('click', () => {
      const type = btn.dataset.type; // 'button', 'pressed', 'stick', 'ring', 'bg'
      const color = btn.dataset.color;

      // Unselect siblings
      btn.parentElement.querySelectorAll('.color-option-btn').forEach(b => b.classList.remove('selected'));
      btn.classList.add('selected');

      if (type === 'button') state.buttonColor = color;
      if (type === 'pressed') state.pressedColor = color;
      if (type === 'stick') state.stickColor = color;
      if (type === 'ring') state.ringColor = color;
      if (type === 'bg') state.bgColor = color;

      if (!state.isMulticolor) {
        applyStaticColors();
      }
    });
  });
}

function applyStaticColors() {
  const regularBtns = document.querySelectorAll('.ctrl-btn:not(.pressed), .ctrl-rect:not(.pressed)');
  const sticks = document.querySelectorAll('.ctrl-stick');
  const rings = document.querySelectorAll('.ctrl-stick-ring');
  const controllerBg = document.querySelector('.controller-bg');

  regularBtns.forEach(b => b.style.backgroundColor = state.buttonColor);
  sticks.forEach(s => s.style.backgroundColor = state.stickColor);
  rings.forEach(r => r.style.borderColor = state.ringColor);
  if (controllerBg) controllerBg.style.backgroundColor = state.bgColor;
}

// 4. Clickable Buttons Simulation
function initButtonClicks() {
  const buttons = document.querySelectorAll('.ctrl-btn, .ctrl-rect');
  buttons.forEach(btn => {
    btn.addEventListener('mousedown', (e) => {
      e.stopPropagation();
      btn.classList.add('pressed');
      if (!state.isMulticolor) {
        btn.style.backgroundColor = state.pressedColor;
      }
    });

    const release = () => {
      btn.classList.remove('pressed');
      if (!state.isMulticolor) {
        btn.style.backgroundColor = state.buttonColor;
      }
    };

    btn.addEventListener('mouseup', release);
    btn.addEventListener('mouseleave', release);
  });
}

// 5. Tabs
function initTabs() {
  const tabs = document.querySelectorAll('.tab-btn');
  const contents = document.querySelectorAll('.tab-content');

  tabs.forEach(tab => {
    tab.addEventListener('click', () => {
      const target = tab.dataset.tab;

      tabs.forEach(t => t.classList.remove('active'));
      contents.forEach(c => c.classList.remove('active'));

      tab.classList.add('active');
      const content = document.getElementById(target);
      if (content) content.classList.add('active');
    });
  });
}

// 6. Copy Buttons
function initCopyButtons() {
  document.querySelectorAll('.copy-btn').forEach(btn => {
    btn.addEventListener('click', () => {
      const textToCopy = btn.dataset.copy;
      if (textToCopy) {
        navigator.clipboard.writeText(textToCopy).then(() => {
          const origText = btn.textContent;
          btn.textContent = '✓ Copié !';
          btn.style.background = 'rgba(16, 185, 129, 0.3)';
          btn.style.color = '#10b981';
          setTimeout(() => {
            btn.textContent = origText;
            btn.style.background = '';
            btn.style.color = '';
          }, 2000);
        });
      }
    });
  });
}

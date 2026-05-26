#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include <DNSServer.h>
#include <SD.h>
#include <Update.h>           // OTA firmware updates via web

// AP Mode Configuration
const char* AP_SSID = "WeatherStation-Setup";
const char* AP_PASSWORD = "weather123";  // Min 8 characters
const IPAddress AP_IP(192, 168, 4, 1);
const IPAddress AP_GATEWAY(192, 168, 4, 1);
const IPAddress AP_SUBNET(255, 255, 255, 0);

// DNS server for captive portal
DNSServer dnsServer;
const byte DNS_PORT = 53;

// Web server
WebServer webServer(80);

// Preferences for persistent storage
Preferences preferences;

// Flag to force AP mode (stored in preferences)
bool forceAPMode = false;

// Track web activity to prevent sleep during configuration
unsigned long lastWebRequestTime = 0;
const unsigned long WEB_ACTIVITY_TIMEOUT = 120000;  // 2 minutes without web activity

// Runtime config - loaded from preferences or defaults
struct ConfigData {
  char wifi_ssid[33];
  char wifi_password[65];
  char wifi_ssid2[33];
  char wifi_password2[65];
  char wifi_ssid3[33];
  char wifi_password3[65];
  char api_key[65];
  int forecast_days;        // 3 or 5 days forecast
  char city[33];
  char latitude[16];
  char longitude[16];
  char language[8];
  char hemisphere[8];
  char units[4];
  char timezone[32];
  int gmt_offset;
  int dst_offset;
  int update_interval;      // Weather update interval in minutes
  int sleep_timeout;        // Time before deep sleep in seconds
  bool keep_screen_on_sleep; // Keep current screen when entering deep sleep
  int wakeup_hour;          // Hour to start updating (0-23)
  int sleep_hour;           // Hour to stop updating (0-23)
} config;

// HTML page for configuration - Modern responsive design with tabs
const char CONFIG_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="es">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Weather Station</title>
  <style>
    * { box-sizing: border-box; margin: 0; padding: 0; }
    :root {
      --primary: #3b82f6;
      --primary-dark: #2563eb;
      --success: #10b981;
      --danger: #ef4444;
      --warning: #f59e0b;
      --bg: #0f172a;
      --bg-card: #1e293b;
      --bg-input: #334155;
      --text: #f1f5f9;
      --text-muted: #94a3b8;
      --border: #475569;
    }
    body {
      font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
      background: var(--bg);
      color: var(--text);
      min-height: 100vh;
      padding: 16px;
    }
    .container {
      max-width: 600px;
      margin: 0 auto;
    }
    header {
      text-align: center;
      padding: 20px 0;
      margin-bottom: 20px;
    }
    header h1 {
      font-size: 1.5rem;
      font-weight: 600;
      display: flex;
      align-items: center;
      justify-content: center;
      gap: 10px;
    }
    header h1::before {
      content: "☀️";
    }
    .status {
      font-size: 0.8rem;
      color: var(--success);
      margin-top: 8px;
    }
    .tabs {
      display: flex;
      gap: 4px;
      margin-bottom: 20px;
      overflow-x: auto;
      padding-bottom: 4px;
    }
    .tab {
      flex: 1;
      min-width: 80px;
      padding: 12px 8px;
      background: var(--bg-card);
      border: none;
      border-radius: 8px 8px 0 0;
      color: var(--text-muted);
      font-size: 0.85rem;
      cursor: pointer;
      transition: all 0.2s;
      white-space: nowrap;
    }
    .tab:hover { background: var(--bg-input); }
    .tab.active {
      background: var(--primary);
      color: white;
    }
    .panel {
      display: none;
      background: var(--bg-card);
      border-radius: 0 0 12px 12px;
      padding: 20px;
      animation: fadeIn 0.3s;
    }
    .panel.active { display: block; }
    @keyframes fadeIn {
      from { opacity: 0; transform: translateY(-10px); }
      to { opacity: 1; transform: translateY(0); }
    }
    .form-group {
      margin-bottom: 16px;
    }
    .form-row {
      display: grid;
      grid-template-columns: 1fr 1fr;
      gap: 12px;
    }
    @media (max-width: 480px) {
      .form-row { grid-template-columns: 1fr; }
    }
    label {
      display: block;
      font-size: 0.85rem;
      font-weight: 500;
      margin-bottom: 6px;
      color: var(--text);
    }
    input, select {
      width: 100%;
      padding: 12px;
      background: var(--bg-input);
      border: 1px solid var(--border);
      border-radius: 8px;
      color: var(--text);
      font-size: 1rem;
      transition: border-color 0.2s;
    }
    input:focus, select:focus {
      outline: none;
      border-color: var(--primary);
    }
    input::placeholder { color: var(--text-muted); }
    select { cursor: pointer; }
    select option { background: var(--bg-card); }
    .hint {
      font-size: 0.75rem;
      color: var(--text-muted);
      margin-top: 4px;
    }
    .card {
      background: var(--bg-input);
      border-radius: 8px;
      padding: 16px;
      margin-bottom: 16px;
    }
    .card-title {
      font-size: 0.9rem;
      font-weight: 600;
      margin-bottom: 12px;
      color: var(--primary);
    }
    .btn-group {
      display: flex;
      flex-direction: column;
      gap: 10px;
      margin-top: 24px;
    }
    button {
      width: 100%;
      padding: 14px;
      border: none;
      border-radius: 8px;
      font-size: 1rem;
      font-weight: 600;
      cursor: pointer;
      transition: transform 0.1s, opacity 0.2s;
    }
    button:active { transform: scale(0.98); }
    .btn-primary { background: var(--primary); color: white; }
    .btn-primary:hover { background: var(--primary-dark); }
    .btn-success { background: var(--success); color: white; }
    .btn-danger { background: var(--danger); color: white; opacity: 0.8; }
    .btn-danger:hover { opacity: 1; }
    .divider {
      height: 1px;
      background: var(--border);
      margin: 20px 0;
    }
    .wifi-network {
      border: 1px solid var(--border);
      border-radius: 8px;
      padding: 12px;
      margin-bottom: 12px;
    }
    .wifi-network:last-child { margin-bottom: 0; }
    .network-header {
      font-size: 0.8rem;
      color: var(--text-muted);
      margin-bottom: 8px;
    }
  </style>
</head>
<body>
  <div class="container">
    <header>
      <h1>Weather Station</h1>
      <div class="status">● Configuracion</div>
    </header>

    <div class="tabs">
      <button class="tab active" onclick="showTab('wifi')">WiFi</button>
      <button class="tab" onclick="showTab('weather')">Clima</button>
      <button class="tab" onclick="showTab('display')">Display</button>
      <button class="tab" onclick="showTab('system')">Sistema</button>
    </div>

    <form id="configForm" action="/save" method="POST">
      <!-- WiFi Tab -->
      <div id="wifi" class="panel active">
        <div class="wifi-network">
          <div class="network-header">Red Principal</div>
          <div class="form-group">
            <label>SSID</label>
            <input type="text" name="ssid1" value="%SSID1%" placeholder="Nombre de red" maxlength="32">
          </div>
          <div class="form-group">
            <label>Contrasena</label>
            <input type="password" name="pass1" value="%PASS1%" placeholder="••••••••" maxlength="64">
          </div>
        </div>

        <div class="wifi-network">
          <div class="network-header">Red Secundaria (opcional)</div>
          <div class="form-group">
            <label>SSID</label>
            <input type="text" name="ssid2" value="%SSID2%" placeholder="Nombre de red" maxlength="32">
          </div>
          <div class="form-group">
            <label>Contrasena</label>
            <input type="password" name="pass2" value="%PASS2%" placeholder="••••••••" maxlength="64">
          </div>
        </div>

        <div class="wifi-network">
          <div class="network-header">Red Terciaria (opcional)</div>
          <div class="form-group">
            <label>SSID</label>
            <input type="text" name="ssid3" value="%SSID3%" placeholder="Nombre de red" maxlength="32">
          </div>
          <div class="form-group">
            <label>Contrasena</label>
            <input type="password" name="pass3" value="%PASS3%" placeholder="••••••••" maxlength="64">
          </div>
        </div>
      </div>

      <!-- Weather Tab -->
      <div id="weather" class="panel">
        <div class="card">
          <div class="card-title">WeatherAPI.com</div>
          <div class="form-group">
            <label>API Key</label>
            <input type="text" name="apikey" value="%APIKEY%" placeholder="Tu API key" maxlength="64">
            <div class="hint">Obten tu key gratis en weatherapi.com</div>
          </div>
          <div class="form-group">
            <label>Dias de Pronostico</label>
            <select name="forecast_days">
              <option value="3" %FORECAST3_SEL%>3 Dias</option>
              <option value="5" %FORECAST5_SEL%>5 Dias</option>
            </select>
          </div>
        </div>

        <div class="card">
          <div class="card-title">Ubicacion</div>
          <div class="form-group">
            <label>Ciudad</label>
            <input type="text" name="city" value="%CITY%" placeholder="Nombre de ciudad" maxlength="32">
          </div>
          <div class="form-row">
            <div class="form-group">
              <label>Latitud</label>
              <input type="text" name="lat" value="%LAT%" placeholder="19.4326" maxlength="15">
            </div>
            <div class="form-group">
              <label>Longitud</label>
              <input type="text" name="lon" value="%LON%" placeholder="-99.1332" maxlength="15">
            </div>
          </div>
          <div class="form-group">
            <label>Hemisferio</label>
            <select name="hemisphere">
              <option value="north" %NORTH_SEL%>Norte</option>
              <option value="south" %SOUTH_SEL%>Sur</option>
            </select>
          </div>
        </div>
      </div>

      <!-- Display Tab -->
      <div id="display" class="panel">
        <div class="card">
          <div class="card-title">Preferencias de Pantalla</div>
          <div class="form-row">
            <div class="form-group">
              <label>Idioma</label>
              <select name="lang">
                <option value="ES" %LANG_ES_SEL%>Espanol</option>
                <option value="EN" %LANG_EN_SEL%>English</option>
                <option value="FR" %LANG_FR_SEL%>Francais</option>
              </select>
            </div>
            <div class="form-group">
              <label>Unidades</label>
              <select name="units">
                <option value="M" %METRIC_SEL%>Metrico (°C)</option>
                <option value="I" %IMPERIAL_SEL%>Imperial (°F)</option>
              </select>
            </div>
          </div>
        </div>

        <div class="card">
          <div class="card-title">Zona Horaria</div>
          <div class="form-group">
            <label>Timezone</label>
            <input type="text" name="tz" value="%TZ%" placeholder="CST6" maxlength="31">
            <div class="hint">Ej: CST6, EST5EDT, GMT0, CET-1</div>
          </div>
          <div class="form-row">
            <div class="form-group">
              <label>GMT Offset (seg)</label>
              <input type="number" name="gmt" value="%GMT%">
              <div class="hint">-21600 para CST</div>
            </div>
            <div class="form-group">
              <label>DST Offset (seg)</label>
              <input type="number" name="dst" value="%DST%">
              <div class="hint">0 o 3600</div>
            </div>
          </div>
        </div>
      </div>

      <!-- System Tab -->
      <div id="system" class="panel">
        <div class="card">
          <div class="card-title">Actualizacion de Datos</div>
          <div class="form-group">
            <label>Intervalo de Actualizacion (min)</label>
            <input type="number" name="update_interval" value="%UPDATE_INT%" min="5" max="120">
            <div class="hint">Cada cuanto actualizar el clima (5-120 min)</div>
          </div>
          <div class="form-group">
            <label>Tiempo antes de Sleep (seg)</label>
            <input type="number" name="sleep_timeout" value="%SLEEP_TIME%" min="10" max="300">
            <div class="hint">Tiempo de inactividad antes de dormir (10-300 seg)</div>
          </div>
          <div class="form-group">
            <label>Al entrar en Sleep</label>
            <select name="keep_screen">
              <option value="0" %KEEP_SCREEN_OFF%>Regresar a pantalla principal</option>
              <option value="1" %KEEP_SCREEN_ON%>Mantener pantalla actual</option>
            </select>
            <div class="hint">Comportamiento al entrar en modo de bajo consumo</div>
          </div>
        </div>

        <div class="card">
          <div class="card-title">Horario de Actividad (Ahorro de Bateria)</div>
          <div class="form-group">
            <label>Hora de Inicio (despertar)</label>
            <input type="number" name="wakeup_hour" value="%WAKEUP_HOUR%" min="0" max="23">
            <div class="hint">Hora a partir de la cual actualiza el clima (0-23)</div>
          </div>
          <div class="form-group">
            <label>Hora de Fin (dormir)</label>
            <input type="number" name="sleep_hour" value="%SLEEP_HOUR%" min="0" max="23">
            <div class="hint">Hora a partir de la cual deja de actualizar (0-23)</div>
          </div>
          <div class="hint" style="margin-top:10px;padding:10px;background:#e3f2fd;border-radius:4px;">
            <b>Ejemplo:</b> Inicio=7, Fin=23 = Activo de 7:00 a 23:00<br>
            <b>Nota:</b> Si Inicio > Fin, funciona durante la noche (ej: 22 a 6)
          </div>
        </div>

        <div class="divider"></div>

        <div class="btn-group">
          <button type="submit" class="btn-primary">Guardar Configuracion</button>
          <button type="submit" formaction="/reboot" class="btn-success">Guardar y Reiniciar</button>
          <button type="submit" formaction="/reset" class="btn-danger" onclick="return confirm('¿Restablecer a valores de fabrica?')">Restablecer Fabrica</button>
        </div>
      </div>
    </form>
  </div>

  <script>
    function showTab(tabId) {
      document.querySelectorAll('.tab').forEach(t => t.classList.remove('active'));
      document.querySelectorAll('.panel').forEach(p => p.classList.remove('active'));
      document.querySelector(`[onclick="showTab('${tabId}')"]`).classList.add('active');
      document.getElementById(tabId).classList.add('active');
    }
  </script>
</body>
</html>
)rawliteral";

const char SAVE_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Guardado</title>
  <style>
    * { box-sizing: border-box; }
    body { font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif; display: flex; justify-content: center; align-items: center; min-height: 100vh; margin: 0; background: #0f172a; color: #f1f5f9; }
    .container { text-align: center; background: #1e293b; padding: 40px; border-radius: 16px; max-width: 400px; margin: 16px; }
    .icon { font-size: 4rem; margin-bottom: 16px; }
    h1 { color: #10b981; font-size: 1.5rem; margin-bottom: 12px; }
    p { color: #94a3b8; margin-bottom: 24px; }
    a { display: inline-block; padding: 12px 24px; background: #3b82f6; color: white; text-decoration: none; border-radius: 8px; font-weight: 600; }
    a:hover { background: #2563eb; }
  </style>
</head>
<body>
  <div class="container">
    <div class="icon">✓</div>
    <h1>Configuracion Guardada</h1>
    <p>Los cambios se han guardado correctamente.</p>
    <a href="/">Volver a Configuracion</a>
  </div>
</body>
</html>
)rawliteral";

const char REBOOT_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Reiniciando</title>
  <style>
    * { box-sizing: border-box; }
    body { font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif; display: flex; justify-content: center; align-items: center; min-height: 100vh; margin: 0; background: #0f172a; color: #f1f5f9; }
    .container { text-align: center; background: #1e293b; padding: 40px; border-radius: 16px; max-width: 400px; margin: 16px; }
    .spinner { width: 50px; height: 50px; border: 4px solid #334155; border-top-color: #3b82f6; border-radius: 50%; animation: spin 1s linear infinite; margin: 0 auto 20px; }
    @keyframes spin { to { transform: rotate(360deg); } }
    h1 { color: #3b82f6; font-size: 1.5rem; margin-bottom: 12px; }
    p { color: #94a3b8; margin-bottom: 8px; }
  </style>
</head>
<body>
  <div class="container">
    <div class="spinner"></div>
    <h1>Reiniciando...</h1>
    <p>El dispositivo se reiniciara y conectara a tu red WiFi.</p>
    <p>La pantalla del clima se actualizara automaticamente.</p>
  </div>
</body>
</html>
)rawliteral";

// OTA Update page - firmware upload via web interface
const char OTA_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="es">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Firmware Update</title>
  <style>
    * { box-sizing: border-box; margin: 0; padding: 0; }
    :root {
      --primary: #3b82f6;
      --primary-dark: #2563eb;
      --success: #10b981;
      --danger: #ef4444;
      --bg: #0f172a;
      --bg-card: #1e293b;
      --text: #f1f5f9;
      --text-muted: #94a3b8;
    }
    body {
      font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
      background: var(--bg);
      color: var(--text);
      min-height: 100vh;
      padding: 16px;
      display: flex;
      align-items: center;
      justify-content: center;
    }
    .container { max-width: 500px; width: 100%; }
    .card {
      background: var(--bg-card);
      border-radius: 16px;
      padding: 32px;
      text-align: center;
    }
    h1 { color: var(--primary); font-size: 1.75rem; margin-bottom: 8px; }
    .version { color: var(--text-muted); font-size: 0.9rem; margin-bottom: 24px; }
    .upload-area {
      border: 2px dashed #475569;
      border-radius: 12px;
      padding: 32px;
      margin-bottom: 20px;
      transition: all 0.3s;
    }
    .upload-area:hover { border-color: var(--primary); background: rgba(59, 130, 246, 0.1); }
    .upload-icon { font-size: 48px; margin-bottom: 12px; }
    input[type="file"] { display: none; }
    .btn {
      display: inline-block;
      padding: 12px 32px;
      border-radius: 8px;
      font-size: 1rem;
      font-weight: 600;
      cursor: pointer;
      border: none;
      transition: all 0.2s;
      text-decoration: none;
    }
    .btn-primary { background: var(--primary); color: white; }
    .btn-primary:hover { background: var(--primary-dark); }
    .btn-primary:disabled { background: #475569; cursor: not-allowed; }
    .btn-secondary { background: transparent; color: var(--text-muted); border: 1px solid #475569; margin-top: 12px; }
    .progress { display: none; margin-top: 20px; }
    .progress-bar { height: 8px; background: #334155; border-radius: 4px; overflow: hidden; }
    .progress-fill { height: 100%; background: var(--primary); width: 0%; transition: width 0.3s; }
    .progress-text { margin-top: 8px; color: var(--text-muted); font-size: 0.9rem; }
    .status { margin-top: 16px; padding: 12px; border-radius: 8px; display: none; }
    .status.success { display: block; background: rgba(16, 185, 129, 0.2); color: var(--success); }
    .status.error { display: block; background: rgba(239, 68, 68, 0.2); color: var(--danger); }
    .file-info { color: var(--text-muted); font-size: 0.9rem; margin-top: 8px; }
    .warning { background: rgba(245, 158, 11, 0.2); color: #fbbf24; padding: 12px; border-radius: 8px; margin-bottom: 20px; font-size: 0.85rem; }
  </style>
</head>
<body>
  <div class="container">
    <div class="card">
      <h1>Firmware Update</h1>
      <p class="version">Version actual: %VERSION%</p>
      <div class="warning">No desconectes el dispositivo durante la actualizacion</div>
      <form id="uploadForm" enctype="multipart/form-data">
        <div class="upload-area" id="dropZone">
          <div class="upload-icon">📦</div>
          <p>Arrastra el archivo .bin aqui</p>
          <p class="file-info">o haz clic para seleccionar</p>
          <input type="file" id="firmware" name="firmware" accept=".bin">
        </div>
        <p id="fileName" class="file-info"></p>
        <button type="submit" class="btn btn-primary" id="uploadBtn" disabled>Actualizar Firmware</button>
      </form>
      <div class="progress" id="progress">
        <div class="progress-bar"><div class="progress-fill" id="progressFill"></div></div>
        <p class="progress-text" id="progressText">Subiendo... 0%</p>
      </div>
      <div class="status" id="status"></div>
      <a href="/" class="btn btn-secondary">Volver</a>
    </div>
  </div>
  <script>
    const dropZone = document.getElementById('dropZone');
    const fileInput = document.getElementById('firmware');
    const fileName = document.getElementById('fileName');
    const uploadBtn = document.getElementById('uploadBtn');
    const uploadForm = document.getElementById('uploadForm');
    const progress = document.getElementById('progress');
    const progressFill = document.getElementById('progressFill');
    const progressText = document.getElementById('progressText');
    const status = document.getElementById('status');
    dropZone.addEventListener('click', () => fileInput.click());
    dropZone.addEventListener('dragover', (e) => { e.preventDefault(); dropZone.style.borderColor = '#3b82f6'; });
    dropZone.addEventListener('dragleave', () => dropZone.style.borderColor = '#475569');
    dropZone.addEventListener('drop', (e) => { e.preventDefault(); dropZone.style.borderColor = '#475569'; if (e.dataTransfer.files.length) { fileInput.files = e.dataTransfer.files; updateFileName(); } });
    fileInput.addEventListener('change', updateFileName);
    function updateFileName() { if (fileInput.files.length) { fileName.textContent = fileInput.files[0].name + ' (' + (fileInput.files[0].size / 1024).toFixed(1) + ' KB)'; uploadBtn.disabled = false; } }
    uploadForm.addEventListener('submit', async (e) => {
      e.preventDefault();
      if (!fileInput.files.length) return;
      const formData = new FormData();
      formData.append('firmware', fileInput.files[0]);
      uploadBtn.disabled = true;
      progress.style.display = 'block';
      status.className = 'status';
      const xhr = new XMLHttpRequest();
      xhr.open('POST', '/update', true);
      xhr.upload.onprogress = (e) => { if (e.lengthComputable) { const pct = Math.round((e.loaded / e.total) * 100); progressFill.style.width = pct + '%'; progressText.textContent = 'Subiendo... ' + pct + '%'; } };
      xhr.onload = () => { if (xhr.status === 200) { progressText.textContent = 'Reiniciando...'; status.className = 'status success'; status.textContent = 'Actualizacion exitosa. Reiniciando...'; setTimeout(() => location.href = '/', 5000); } else { status.className = 'status error'; status.textContent = 'Error: ' + xhr.responseText; uploadBtn.disabled = false; } };
      xhr.onerror = () => { status.className = 'status error'; status.textContent = 'Error de conexion'; uploadBtn.disabled = false; };
      xhr.send(formData);
    });
  </script>
</body>
</html>
)rawliteral";

// OTA update state
bool otaInProgress = false;

// Handle OTA page
void handleOTA() {
  lastWebRequestTime = millis();
  extern String version;
  String html = String(OTA_PAGE);
  html.replace("%VERSION%", version);
  webServer.send(200, "text/html", html);
}

// Handle firmware upload
void handleOTAUpload() {
  HTTPUpload& upload = webServer.upload();
  if (upload.status == UPLOAD_FILE_START) {
    Serial.printf("OTA Update Start: %s\n", upload.filename.c_str());
    otaInProgress = true;
    if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { Update.printError(Serial); }
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) { Update.printError(Serial); }
  } else if (upload.status == UPLOAD_FILE_END) {
    if (Update.end(true)) { Serial.printf("OTA Update Success: %u bytes\n", upload.totalSize); }
    else { Update.printError(Serial); }
    otaInProgress = false;
  }
}

// Handle upload completion
void handleOTAResult() {
  lastWebRequestTime = millis();
  if (Update.hasError()) {
    webServer.send(500, "text/plain", Update.errorString());
  } else {
    webServer.send(200, "text/plain", "OK");
    delay(1000);
    ESP.restart();
  }
}

// Load configuration from preferences (uses defaults from credentials.h if not saved)
void loadConfig() {
  preferences.begin("weather", true);  // Read-only

  // Load saved values, use defaults from credentials.h if empty
  String savedSsid1 = preferences.getString("ssid1", "");
  if (savedSsid1.length() > 0) {
    strlcpy(config.wifi_ssid, savedSsid1.c_str(), sizeof(config.wifi_ssid));
  } else if (wifiNetworkCount > 0) {
    strlcpy(config.wifi_ssid, wifiNetworks[0].ssid, sizeof(config.wifi_ssid));
  }

  String savedPass1 = preferences.getString("pass1", "");
  if (savedPass1.length() > 0) {
    strlcpy(config.wifi_password, savedPass1.c_str(), sizeof(config.wifi_password));
  } else if (wifiNetworkCount > 0) {
    strlcpy(config.wifi_password, wifiNetworks[0].password, sizeof(config.wifi_password));
  }

  String savedSsid2 = preferences.getString("ssid2", "");
  if (savedSsid2.length() > 0) {
    strlcpy(config.wifi_ssid2, savedSsid2.c_str(), sizeof(config.wifi_ssid2));
  } else if (wifiNetworkCount > 1) {
    strlcpy(config.wifi_ssid2, wifiNetworks[1].ssid, sizeof(config.wifi_ssid2));
  }

  String savedPass2 = preferences.getString("pass2", "");
  if (savedPass2.length() > 0) {
    strlcpy(config.wifi_password2, savedPass2.c_str(), sizeof(config.wifi_password2));
  } else if (wifiNetworkCount > 1) {
    strlcpy(config.wifi_password2, wifiNetworks[1].password, sizeof(config.wifi_password2));
  }

  String savedSsid3 = preferences.getString("ssid3", "");
  if (savedSsid3.length() > 0) {
    strlcpy(config.wifi_ssid3, savedSsid3.c_str(), sizeof(config.wifi_ssid3));
  } else if (wifiNetworkCount > 2) {
    strlcpy(config.wifi_ssid3, wifiNetworks[2].ssid, sizeof(config.wifi_ssid3));
  }

  String savedPass3 = preferences.getString("pass3", "");
  if (savedPass3.length() > 0) {
    strlcpy(config.wifi_password3, savedPass3.c_str(), sizeof(config.wifi_password3));
  } else if (wifiNetworkCount > 2) {
    strlcpy(config.wifi_password3, wifiNetworks[2].password, sizeof(config.wifi_password3));
  }

  String savedApiKey = preferences.getString("apikey", "");
  if (savedApiKey.length() > 0) {
    strlcpy(config.api_key, savedApiKey.c_str(), sizeof(config.api_key));
  } else {
    strlcpy(config.api_key, apikey.c_str(), sizeof(config.api_key));
  }

  config.forecast_days = preferences.getInt("fcdays", 3);

  String savedCity = preferences.getString("city", "");
  if (savedCity.length() > 0) {
    strlcpy(config.city, savedCity.c_str(), sizeof(config.city));
  } else {
    strlcpy(config.city, City.c_str(), sizeof(config.city));
  }

  String savedLat = preferences.getString("lat", "");
  if (savedLat.length() > 0) {
    strlcpy(config.latitude, savedLat.c_str(), sizeof(config.latitude));
  } else {
    strlcpy(config.latitude, Latitude.c_str(), sizeof(config.latitude));
  }

  String savedLon = preferences.getString("lon", "");
  if (savedLon.length() > 0) {
    strlcpy(config.longitude, savedLon.c_str(), sizeof(config.longitude));
  } else {
    strlcpy(config.longitude, Longitude.c_str(), sizeof(config.longitude));
  }

  String savedLang = preferences.getString("lang", "");
  if (savedLang.length() > 0) {
    strlcpy(config.language, savedLang.c_str(), sizeof(config.language));
  } else {
    strlcpy(config.language, Language.c_str(), sizeof(config.language));
  }

  String savedHemi = preferences.getString("hemi", "");
  if (savedHemi.length() > 0) {
    strlcpy(config.hemisphere, savedHemi.c_str(), sizeof(config.hemisphere));
  } else {
    strlcpy(config.hemisphere, Hemisphere.c_str(), sizeof(config.hemisphere));
  }

  String savedUnits = preferences.getString("units", "");
  if (savedUnits.length() > 0) {
    strlcpy(config.units, savedUnits.c_str(), sizeof(config.units));
  } else {
    strlcpy(config.units, Units.c_str(), sizeof(config.units));
  }

  String savedTz = preferences.getString("tz", "");
  if (savedTz.length() > 0) {
    strlcpy(config.timezone, savedTz.c_str(), sizeof(config.timezone));
  } else {
    strlcpy(config.timezone, Timezone, sizeof(config.timezone));
  }

  config.gmt_offset = preferences.getInt("gmt", gmtOffset_sec);
  config.dst_offset = preferences.getInt("dst", daylightOffset_sec);
  config.update_interval = preferences.getInt("updint", 60);
  config.sleep_timeout = preferences.getInt("sleept", 30);
  config.keep_screen_on_sleep = preferences.getBool("keepscr", false);
  config.wakeup_hour = preferences.getInt("wakeuph", 7);
  config.sleep_hour = preferences.getInt("sleeph", 1);
  forceAPMode = preferences.getBool("forceAP", false);

  preferences.end();

  // Validate ranges
  if (config.update_interval < 5) config.update_interval = 5;
  if (config.update_interval > 120) config.update_interval = 120;
  if (config.sleep_timeout < 10) config.sleep_timeout = 10;
  if (config.sleep_timeout > 300) config.sleep_timeout = 300;
  if (config.wakeup_hour < 0) config.wakeup_hour = 0;
  if (config.wakeup_hour > 23) config.wakeup_hour = 23;
  if (config.sleep_hour < 0) config.sleep_hour = 0;
  if (config.sleep_hour > 23) config.sleep_hour = 23;
}

// Save configuration to preferences
void saveConfig() {
  preferences.begin("weather", false);  // Read-write

  preferences.putString("ssid1", config.wifi_ssid);
  preferences.putString("pass1", config.wifi_password);
  preferences.putString("ssid2", config.wifi_ssid2);
  preferences.putString("pass2", config.wifi_password2);
  preferences.putString("ssid3", config.wifi_ssid3);
  preferences.putString("pass3", config.wifi_password3);
  preferences.putString("apikey", config.api_key);
  preferences.putInt("fcdays", config.forecast_days);
  preferences.putString("city", config.city);
  preferences.putString("lat", config.latitude);
  preferences.putString("lon", config.longitude);
  preferences.putString("lang", config.language);
  preferences.putString("hemi", config.hemisphere);
  preferences.putString("units", config.units);
  preferences.putString("tz", config.timezone);
  preferences.putInt("gmt", config.gmt_offset);
  preferences.putInt("dst", config.dst_offset);
  preferences.putInt("updint", config.update_interval);
  preferences.putInt("sleept", config.sleep_timeout);
  preferences.putBool("keepscr", config.keep_screen_on_sleep);
  preferences.putInt("wakeuph", config.wakeup_hour);
  preferences.putInt("sleeph", config.sleep_hour);
  preferences.putBool("forceAP", false);  // Clear force AP flag after save

  preferences.end();
}

// Reset to factory defaults
void resetConfig() {
  preferences.begin("weather", false);
  preferences.clear();
  preferences.end();
}

// Set force AP mode flag
void setForceAPMode(bool force) {
  preferences.begin("weather", false);
  preferences.putBool("forceAP", force);
  preferences.end();
  forceAPMode = force;
}

// Check if configuration exists
bool hasValidConfig() {
  return strlen(config.wifi_ssid) > 0 && strlen(config.api_key) > 0;
}

// Process template placeholders
String processTemplate(const char* page) {
  String html = String(page);

  html.replace("%SSID1%", config.wifi_ssid);
  html.replace("%PASS1%", config.wifi_password);
  html.replace("%SSID2%", config.wifi_ssid2);
  html.replace("%PASS2%", config.wifi_password2);
  html.replace("%SSID3%", config.wifi_ssid3);
  html.replace("%PASS3%", config.wifi_password3);
  html.replace("%APIKEY%", config.api_key);
  html.replace("%FORECAST3_SEL%", config.forecast_days == 3 ? "selected" : "");
  html.replace("%FORECAST5_SEL%", config.forecast_days == 5 ? "selected" : "");
  html.replace("%CITY%", config.city);
  html.replace("%LAT%", config.latitude);
  html.replace("%LON%", config.longitude);
  html.replace("%LANG_ES_SEL%", strcmp(config.language, "ES") == 0 ? "selected" : "");
  html.replace("%LANG_EN_SEL%", strcmp(config.language, "EN") == 0 ? "selected" : "");
  html.replace("%LANG_FR_SEL%", strcmp(config.language, "FR") == 0 ? "selected" : "");
  html.replace("%TZ%", config.timezone);
  html.replace("%GMT%", String(config.gmt_offset));
  html.replace("%DST%", String(config.dst_offset));
  html.replace("%UPDATE_INT%", String(config.update_interval));
  html.replace("%SLEEP_TIME%", String(config.sleep_timeout));

  // Hemisphere selection
  html.replace("%NORTH_SEL%", strcmp(config.hemisphere, "north") == 0 ? "selected" : "");
  html.replace("%SOUTH_SEL%", strcmp(config.hemisphere, "south") == 0 ? "selected" : "");

  // Units selection
  html.replace("%METRIC_SEL%", strcmp(config.units, "M") == 0 ? "selected" : "");
  html.replace("%IMPERIAL_SEL%", strcmp(config.units, "I") == 0 ? "selected" : "");

  // Keep screen on sleep selection
  html.replace("%KEEP_SCREEN_OFF%", config.keep_screen_on_sleep ? "" : "selected");
  html.replace("%KEEP_SCREEN_ON%", config.keep_screen_on_sleep ? "selected" : "");

  // Activity hours
  html.replace("%WAKEUP_HOUR%", String(config.wakeup_hour));
  html.replace("%SLEEP_HOUR%", String(config.sleep_hour));

  return html;
}

// Web server handlers
void handleRoot() {
  lastWebRequestTime = millis();  // Track web activity
  String html = processTemplate(CONFIG_PAGE);
  webServer.send(200, "text/html", html);
}

void handleSave() {
  lastWebRequestTime = millis();  // Track web activity
  // Get form values
  strlcpy(config.wifi_ssid, webServer.arg("ssid1").c_str(), sizeof(config.wifi_ssid));
  strlcpy(config.wifi_password, webServer.arg("pass1").c_str(), sizeof(config.wifi_password));
  strlcpy(config.wifi_ssid2, webServer.arg("ssid2").c_str(), sizeof(config.wifi_ssid2));
  strlcpy(config.wifi_password2, webServer.arg("pass2").c_str(), sizeof(config.wifi_password2));
  strlcpy(config.wifi_ssid3, webServer.arg("ssid3").c_str(), sizeof(config.wifi_ssid3));
  strlcpy(config.wifi_password3, webServer.arg("pass3").c_str(), sizeof(config.wifi_password3));
  strlcpy(config.api_key, webServer.arg("apikey").c_str(), sizeof(config.api_key));
  config.forecast_days = webServer.arg("forecast_days").toInt();
  strlcpy(config.city, webServer.arg("city").c_str(), sizeof(config.city));
  strlcpy(config.latitude, webServer.arg("lat").c_str(), sizeof(config.latitude));
  strlcpy(config.longitude, webServer.arg("lon").c_str(), sizeof(config.longitude));
  strlcpy(config.language, webServer.arg("lang").c_str(), sizeof(config.language));
  strlcpy(config.hemisphere, webServer.arg("hemisphere").c_str(), sizeof(config.hemisphere));
  strlcpy(config.units, webServer.arg("units").c_str(), sizeof(config.units));
  strlcpy(config.timezone, webServer.arg("tz").c_str(), sizeof(config.timezone));
  config.gmt_offset = webServer.arg("gmt").toInt();
  config.dst_offset = webServer.arg("dst").toInt();
  config.update_interval = webServer.arg("update_interval").toInt();
  config.sleep_timeout = webServer.arg("sleep_timeout").toInt();
  config.keep_screen_on_sleep = webServer.arg("keep_screen") == "1";
  config.wakeup_hour = webServer.arg("wakeup_hour").toInt();
  config.sleep_hour = webServer.arg("sleep_hour").toInt();

  saveConfig();

  webServer.send(200, "text/html", SAVE_PAGE);
}

void handleReboot() {
  lastWebRequestTime = millis();  // Track web activity
  // Save first
  strlcpy(config.wifi_ssid, webServer.arg("ssid1").c_str(), sizeof(config.wifi_ssid));
  strlcpy(config.wifi_password, webServer.arg("pass1").c_str(), sizeof(config.wifi_password));
  strlcpy(config.wifi_ssid2, webServer.arg("ssid2").c_str(), sizeof(config.wifi_ssid2));
  strlcpy(config.wifi_password2, webServer.arg("pass2").c_str(), sizeof(config.wifi_password2));
  strlcpy(config.wifi_ssid3, webServer.arg("ssid3").c_str(), sizeof(config.wifi_ssid3));
  strlcpy(config.wifi_password3, webServer.arg("pass3").c_str(), sizeof(config.wifi_password3));
  strlcpy(config.api_key, webServer.arg("apikey").c_str(), sizeof(config.api_key));
  config.forecast_days = webServer.arg("forecast_days").toInt();
  strlcpy(config.city, webServer.arg("city").c_str(), sizeof(config.city));
  strlcpy(config.latitude, webServer.arg("lat").c_str(), sizeof(config.latitude));
  strlcpy(config.longitude, webServer.arg("lon").c_str(), sizeof(config.longitude));
  strlcpy(config.language, webServer.arg("lang").c_str(), sizeof(config.language));
  strlcpy(config.hemisphere, webServer.arg("hemisphere").c_str(), sizeof(config.hemisphere));
  strlcpy(config.units, webServer.arg("units").c_str(), sizeof(config.units));
  strlcpy(config.timezone, webServer.arg("tz").c_str(), sizeof(config.timezone));
  config.gmt_offset = webServer.arg("gmt").toInt();
  config.dst_offset = webServer.arg("dst").toInt();
  config.update_interval = webServer.arg("update_interval").toInt();
  config.sleep_timeout = webServer.arg("sleep_timeout").toInt();
  config.keep_screen_on_sleep = webServer.arg("keep_screen") == "1";
  config.wakeup_hour = webServer.arg("wakeup_hour").toInt();
  config.sleep_hour = webServer.arg("sleep_hour").toInt();

  saveConfig();

  webServer.send(200, "text/html", REBOOT_PAGE);
  delay(1000);
  ESP.restart();
}

const char RESET_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Reset</title>
  <style>
    * { box-sizing: border-box; }
    body { font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif; display: flex; justify-content: center; align-items: center; min-height: 100vh; margin: 0; background: #0f172a; color: #f1f5f9; }
    .container { text-align: center; background: #1e293b; padding: 40px; border-radius: 16px; max-width: 400px; margin: 16px; }
    .icon { font-size: 4rem; margin-bottom: 16px; }
    h1 { color: #f59e0b; font-size: 1.5rem; margin-bottom: 12px; }
    p { color: #94a3b8; }
  </style>
</head>
<body>
  <div class="container">
    <div class="icon">⚠️</div>
    <h1>Restablecido a Fabrica</h1>
    <p>El dispositivo se reiniciara con la configuracion por defecto.</p>
  </div>
</body>
</html>
)rawliteral";

void handleReset() {
  lastWebRequestTime = millis();  // Track web activity
  resetConfig();
  webServer.send(200, "text/html", RESET_PAGE);
  delay(1000);
  ESP.restart();
}

void handleNotFound() {
  lastWebRequestTime = millis();  // Track web activity
  // Redirect all requests to root (captive portal behavior)
  webServer.sendHeader("Location", "http://" + AP_IP.toString(), true);
  webServer.send(302, "text/plain", "");
}

// Start AP mode with captive portal
void startAPMode() {
  Serial.println("Starting AP Mode...");

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(AP_IP, AP_GATEWAY, AP_SUBNET);
  WiFi.softAP(AP_SSID, AP_PASSWORD);

  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  // Start DNS server for captive portal
  dnsServer.start(DNS_PORT, "*", AP_IP);

  // Setup web server routes
  webServer.on("/", HTTP_GET, handleRoot);
  webServer.on("/save", HTTP_POST, handleSave);
  webServer.on("/reboot", HTTP_POST, handleReboot);
  webServer.on("/reset", HTTP_POST, handleReset);
  webServer.on("/ota", HTTP_GET, handleOTA);
  webServer.on("/update", HTTP_POST, handleOTAResult, handleOTAUpload);
  webServer.onNotFound(handleNotFound);

  webServer.begin();
  Serial.println("Web server started");
  Serial.printf("Connect to WiFi: %s (password: %s)\n", AP_SSID, AP_PASSWORD);
  Serial.println("Then open http://192.168.4.1 in your browser");
}

// Run AP mode loop (call this in main loop when in AP mode)
void handleAPMode() {
  dnsServer.processNextRequest();
  webServer.handleClient();
}

// Stop AP mode
void stopAPMode() {
  webServer.stop();
  dnsServer.stop();
  WiFi.softAPdisconnect(true);
  WiFi.mode(WIFI_STA);
}

// Start web server in STA mode (when connected to WiFi)
bool webServerRunning = false;

bool isWebActive() {
  if (!webServerRunning) return false;
  // Only consider active if there was an actual web request (lastWebRequestTime > 0)
  // and it happened within the timeout period
  if (lastWebRequestTime == 0) return false;
  return (millis() - lastWebRequestTime) < WEB_ACTIVITY_TIMEOUT;
}

void startWebServer() {
  if (webServerRunning) return;

  Serial.println("Starting Web Server in STA mode...");
  Serial.print("Config page at: http://");
  Serial.println(WiFi.localIP());

  // Setup web server routes
  webServer.on("/", HTTP_GET, handleRoot);
  webServer.on("/save", HTTP_POST, handleSave);
  webServer.on("/reboot", HTTP_POST, handleReboot);
  webServer.on("/reset", HTTP_POST, handleReset);
  webServer.on("/ota", HTTP_GET, handleOTA);
  webServer.on("/update", HTTP_POST, handleOTAResult, handleOTAUpload);

  webServer.begin();
  webServerRunning = true;
  Serial.println("Web server started");
  Serial.println("OTA Update available at /ota");
}

// Handle web server requests (call in loop)
void handleWebServer() {
  if (webServerRunning) {
    webServer.handleClient();
  }
}

// Stop web server
void stopWebServer() {
  if (webServerRunning) {
    webServer.stop();
    webServerRunning = false;
  }
}

// Get WiFi credentials as array for connection
struct WiFiCredentialEntry {
  const char* ssid;
  const char* password;
};

int getConfiguredNetworks(WiFiCredentialEntry* networks, int maxNetworks) {
  int count = 0;

  if (strlen(config.wifi_ssid) > 0 && count < maxNetworks) {
    networks[count].ssid = config.wifi_ssid;
    networks[count].password = config.wifi_password;
    count++;
  }
  if (strlen(config.wifi_ssid2) > 0 && count < maxNetworks) {
    networks[count].ssid = config.wifi_ssid2;
    networks[count].password = config.wifi_password2;
    count++;
  }
  if (strlen(config.wifi_ssid3) > 0 && count < maxNetworks) {
    networks[count].ssid = config.wifi_ssid3;
    networks[count].password = config.wifi_password3;
    count++;
  }

  return count;
}

#endif // WIFI_MANAGER_H

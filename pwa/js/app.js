let espIP = "";

document.addEventListener("DOMContentLoaded", function () {
  document
    .getElementById("discoverBtn")
    .addEventListener("click", discoverGreenneeka);
  document
    .getElementById("manualConnectBtn")
    .addEventListener("click", connectManually);
  document.getElementById("toggleLEDBtn").addEventListener("click", toggleLED);
  document.getElementById("sendTextBtn").addEventListener("click", sendText);

  discoverGreenneeka();
});

async function discoverGreenneeka() {
  const discoverBtn = document.getElementById("discoverBtn");
  const discoverStatus = document.getElementById("discoverStatus");

  discoverBtn.disabled = true;
  discoverBtn.innerHTML = '<div class="loading"></div> Discovering...';
  discoverStatus.innerHTML = "";
  clearMessage();

  try {
    const response = await fetch("http://greenneeka.local/api/status", {
      method: "GET",
      signal: AbortSignal.timeout(3000),
    });

    if (response.ok) {
      espIP = "greenneeka.local";
      showControlPanel();
      discoverStatus.innerHTML =
        '<div class="success">✅ Greenneeka found at greenneeka.local</div>';
      showMessage("Successfully connected to Greenneeka!", "success");
    } else {
      throw new Error("HTTP error: " + response.status);
    }
  } catch (error) {
    discoverStatus.innerHTML =
      '<div class="error">❌ Auto-discovery failed</div>';
    showMessage(
      "Could not auto-discover Greenneeka. Try manual connection.",
      "error"
    );
  } finally {
    discoverBtn.disabled = false;
    discoverBtn.innerHTML = "🔍 Auto-Discover Greenneeka";
  }
}

function connectManually() {
  espIP = document.getElementById("ipInput").value.trim();
  if (!espIP) {
    showMessage("Please enter Greenneeka IP address", "error");
    return;
  }

  // Простая валидация IP
  if (!isValidIP(espIP) && espIP !== "greenneeka.local") {
    showMessage("Please enter a valid IP address or hostname", "error");
    return;
  }

  showControlPanel();
  showMessage(`Connecting to ${espIP}...`, "info");

  // Проверяем подключение
  checkConnection();
}

async function checkConnection() {
  try {
    const response = await fetch(`http://${espIP}/api/status`, {
      method: "GET",
      signal: AbortSignal.timeout(3000),
    });

    if (response.ok) {
      showMessage("Successfully connected to Greenneeka!", "success");
    } else {
      throw new Error("HTTP error: " + response.status);
    }
  } catch (error) {
    showMessage(
      "Connected but cannot verify. Some features may not work.",
      "error"
    );
  }
}

function showControlPanel() {
  document.getElementById("controlPanel").style.display = "block";
  document.getElementById("ipInput").value = espIP;
}

async function toggleLED() {
  if (!espIP) {
    showMessage("Please connect to Greenneeka first", "error");
    return;
  }

  try {
    const response = await fetch(`http://${espIP}/api/toggle`, {
      method: "POST",
    });
    const data = await response.json();

    const statusDiv = document.getElementById("statusDiv");
    statusDiv.textContent = `LED: ${data.led ? "ON" : "OFF"}`;
    statusDiv.className = `status ${data.led ? "on" : "off"}`;

    showMessage(`LED ${data.led ? "ON" : "OFF"}`, "success");
  } catch (error) {
    showMessage("Error toggling LED: " + error.message, "error");
  }
}

async function sendText() {
  if (!espIP) {
    showMessage("Please connect to Greenneeka first", "error");
    return;
  }

  const text = document.getElementById("textInput").value.trim();
  if (!text) {
    showMessage("Please enter some text first", "error");
    return;
  }

  try {
    const response = await fetch(`http://${espIP}/api/print`, {
      method: "POST",
      body: text,
    });
    const result = await response.text();
    showMessage("✅ Text sent to Greenneeka!", "success");
    document.getElementById("textInput").value = ""; // Очищаем поле
  } catch (error) {
    showMessage("Error sending text: " + error.message, "error");
  }
}

function showMessage(text, type) {
  const messageDiv = document.getElementById("message");
  messageDiv.textContent = text;
  messageDiv.className = type;
}

function clearMessage() {
  const messageDiv = document.getElementById("message");
  messageDiv.textContent = "";
  messageDiv.className = "";
}

function isValidIP(ip) {
  const ipPattern = /^(\d{1,3}\.){3}\d{1,3}$/;
  if (!ipPattern.test(ip)) return false;

  const parts = ip.split(".");
  return parts.every((part) => {
    const num = parseInt(part, 10);
    return num >= 0 && num <= 255;
  });
}

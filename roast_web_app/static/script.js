let state = "idle";

function handleButton() {
  if (state === "idle") {
    fetch('/preheat', { method: 'POST' });
    state = "pre";
    document.getElementById("mainBtn").innerText = "Start";
  } else if (state === "pre") {
    fetch('/start', { method: 'POST' });
    state = "running";
    document.getElementById("mainBtn").innerText = "Stop";
  } else {
    fetch('/stop', { method: 'POST' });
    state = "idle";
    document.getElementById("mainBtn").innerText = "Preheat";
  }
}

setInterval(() => {
  fetch('/status')
    .then(res => res.json())
    .then(data => {
      document.getElementById("temp").innerText =
        "Temp: " + data.temp_f + " °F";

      document.getElementById("stage").innerText =
        "Stage: " + data.stage.toUpperCase();

      if (data.start_time) {
        let elapsed = Date.now() / 1000 - data.start_time;
        document.getElementById("timer").innerText =
          elapsed.toFixed(1) + " s";
      }
    });
}, 1000);

async function resetButton() {
  try {
    await fetch('/reset', { method: 'POST' });

    // Reset frontend state
    state = "idle";

    // Reset UI
    document.getElementById("mainBtn").innerText = "Preheat";
    document.getElementById("timer").innerText = "0.0 s";
    document.getElementById("stage").innerText = "Stage: IDLE";
  } catch (err) {
    console.error("Reset failed:", err);
  }
}


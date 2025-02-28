document.addEventListener("DOMContentLoaded", async () => {
    const accidentDiv = document.getElementById("accidents");
    const accidents = await fetchAccidents();
  
    if (!accidents) return (accidentDiv.innerHTML = "No accidents reported.");
  
    Object.entries(accidents).forEach(([key, accident]) => {
      const div = document.createElement("div");
      div.classList.add("p-3", "bg-red-100", "mt-2", "rounded");
  
      div.innerHTML = `
        <p><strong>Rider:</strong> ${accident.rider}</p>
        <p><strong>Location:</strong> ${accident.location}</p>
        <p><strong>Time:</strong> ${accident.time}</p>
        <button class="send-alert bg-red-500 text-white p-2 rounded" data-id="${key}">Send Alert</button>
      `;
      accidentDiv.appendChild(div);
    });
  
    document.querySelectorAll(".send-alert").forEach(button => {
      button.addEventListener("click", async () => {
        await fetch("http://localhost:5000/api/send-alert", {
          method: "POST",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify(accidents[button.dataset.id]),
        });
        alert("Alert sent!");
      });
    });
  });
  
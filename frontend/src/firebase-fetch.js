// firebase Database URL
const firebaseURL = "https://l-guard-default-rtdb.europe-west1.firebasedatabase.app/.json";

// function to fetch data from database
async function fetchData() {
    try {
        const response = await fetch(firebaseURL);
        if (!response.ok) {
            throw new Error("Failed to fetch data");
        }
        const data = await response.json();

        // Extracting Data
        const location = data.location ? `${data.location.latitude}, ${data.location.longitude}` : "No Data";
        const speed = data.speed ? `${data.speed.total.toFixed(2)} km/h` : "No Data";
        
        // it will fetch current timestamp (firebase does not store time)
        const dateTime = new Date().toLocaleString(); 

        // inserting Data into Table
        document.getElementById("location-data").textContent = location;
        document.getElementById("speed-data").textContent = speed;
        document.getElementById("datetime-data").textContent = dateTime;
    } catch (error) {
        console.error("Error fetching data:", error);
    }
}

// call fetchData when the page loads
window.onload = fetchData;

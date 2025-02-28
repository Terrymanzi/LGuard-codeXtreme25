const express = require("express");
const firebaseService = require('../services/firebaseService');
const axios = require("axios");

const router = express.Router();

// Fetch real-time accident data
router.get("/accidents", async (req, res) => {
  try {
    const data = await getAccidentData();
    res.json(data);
  } catch (error) {
    res.status(500).json({ error: "Error fetching data" });
  }
});

// Send emergency alert to hospitals & police
router.post("/send-alert", async (req, res) => {
  try {
    const { location, rider, accidentTime } = req.body;

    await axios.post(process.env.HOSPITAL_API_URL, { location, rider, accidentTime });
    await axios.post(process.env.POLICE_API_URL, { location, rider, accidentTime });

    res.json({ message: "Alert sent successfully" });
  } catch (error) {
    res.status(500).json({ error: "Error sending alert" });
  }
});

module.exports = router;

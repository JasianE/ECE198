import express from "express";
import { readings } from "../data/readings.js";

const router = express.Router();

// GET all readings
router.get("/", (req, res) => {
  res.json(readings);
});

// POST new reading (simulates headband sending data)
router.post("/", (req, res) => {
  const { timestamp, lux } = req.body;
  if (!timestamp || lux === undefined) {
    return res.status(400).json({ message: "Missing timestamp or lux" });
  }
  readings.push({ timestamp, lux });
  res.status(201).json({ success: true });
});

// GET average exposure over last N readings
router.get("/average", (req, res) => {
  const n = parseInt(req.query.n) || readings.length;
  const slice = readings.slice(-n);
  const avg = slice.reduce((sum, r) => sum + r.lux, 0) / slice.length;
  res.json({ averageLux: avg });
});

// GET current risk level (auto-derived)
router.get("/risk", (req, res) => {
  const latest = readings[readings.length - 1];
  let risk = "green";
  if (latest.lux > 700) risk = "red";
  else if (latest.lux > 400) risk = "yellow";
  res.json({ latestLux: latest.lux, risk });
});

export default router;

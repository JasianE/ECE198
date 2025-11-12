// /data/readings.js
// Generates fake light readings for demo / testing

export const readings = (() => {
  const data = [];
  const now = Date.now();

  // Generate 60 readings (1 per minute for the past hour)
  for (let i = 60; i >= 0; i--) {
    data.push({
      timestamp: new Date(now - i * 60 * 1000).toISOString(),
      lux: Math.floor(100 + Math.random() * 600), // 100–700 lux
    });
  }

  return data;
})();

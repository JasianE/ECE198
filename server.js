import express from "express";
import cors from "cors";
import lightRoutes from "./routes/lightRoutes.js";

const app = express();
app.use(cors());
app.use(express.json());

// Mount routes
app.use("/api/light", lightRoutes);

const PORT = 3000;
app.listen(PORT, () => console.log(`✅ Light Tracking API running on port ${PORT}`));

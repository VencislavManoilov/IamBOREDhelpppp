const express = require("express");
const route = express.Router();
const isAuthenticated = require("../middleware/isAuthenticated");

route.get(
    '/',
    isAuthenticated,
    (req, res) => {
        // Destroy session
        req.session.destroy((err) => {
            if (err) {
                console.error('Error destroying session:', err);
                return res.status(500).json({ error: 'Internal Server Error' });
            }
            res.status(200).json({ success: true });
        });
    }
)

module.exports = route;
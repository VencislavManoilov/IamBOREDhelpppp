const express = require("express");
const app = express();
const cors = require("cors");
const mysql = require("mysql2");
const bodyParser = require("body-parser");
const session = require("express-session");
const MySQLStore = require("express-mysql-session")(session);

const PORT = 8080;

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

// Configure MySQL connection
const db = mysql.createConnection({
    host: process.env.MYSQL_HOST || 'localhost',
    user: process.env.MYSQL_USER || 'root',
    password: process.env.MYSQL_PASSWORD || "Asdf1234567890",
    database: process.env.MYSQL_DB || 'code_snippets',
    waitForConnections: true,
    connectionLimit: 10,
    queueLimit: 0
});

// Connect to MySQL
db.connect((err) => {
    if (err) {
        console.error('Error connecting to MySQL:', err);
        return;
    }
    console.log('Connected to MySQL');
});

// Session store setup
const sessionStore = new MySQLStore({
    expiration: 1000 * 60 * 60 * 24 * 7, // 7 days
    endConnectionOnClose: false // Keep the connection open for session store
}, db.promise());

// Session middleware setup
app.use(session({
    key: 'session_cookie_name',
    secret: 'your_secret',
    store: sessionStore,
    resave: false,
    saveUninitialized: false,
    cookie: {
        maxAge: 1000 * 60 * 60 * 24 * 7, // 7 days
        secure: false,
        httpOnly: true
    }
}));

const corsOptions = {
    origin: ['http://localhost:3000', 'http://frontend:3000', 'http://127.0.0.1:3000', process.env.FRONTEND_URL],
    credentials: true,
};

app.use(cors(corsOptions));

app.get("/", (req, res) => {
    res.status(200).json({
        message: "Welcome to the Code Snippets API",
        version: "1.0.0"
    });
});

const signupRoute = require("./routes/signup");
app.use("/signup", (req, res, next) => {
    req.db = db;
    next();
}, signupRoute);

const loginRoute = require("./routes/login");
app.use("/login", (req, res, next) => {
    req.db = db;
    next();
}, loginRoute);

const logoutRoute = require("./routes/logout");
app.use("/logout", (req, res, next) => {
    req.db = db;
    next();
}, logoutRoute);

const createSnippetRoute = require("./routes/createSnippet");
app.use("/snippet/create", (req, res, next) => {
    req.db = db;
    next();
}, createSnippetRoute);

const getSnippetRoute = require("./routes/getSnippet");
app.use("/snippet/get", (req, res, next) => {
    req.db = db;
    next();
}, getSnippetRoute);

const getUserSnippetIds = require("./routes/getUserSnippetIds");
app.use("/snippet/ids", (req, res, next) => {
    req.db = db;
    next();
}, getUserSnippetIds);

const editSnippetRoute = require("./routes/editSnippet");
app.use("/snippet/edit", (req, res, next) => {
    req.db = db;
    next();
}, editSnippetRoute);

const getUserRoute = require("./routes/getUser");
app.use("/user", (req, res, next) => {
    req.db = db;
    next();
}, getUserRoute);

app.get("*", (req, res) => {
    res.status(404).json({
        message: "Not found"
    });
})

app.listen(PORT, () => {
    console.log("Listening to", PORT);
});
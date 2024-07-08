import React, { useState, useEffect } from 'react';
import { BrowserRouter as Router, Route, Routes, Link, useLocation } from 'react-router-dom';
import axios from "axios";
import Navbar from './components/Navbar';
import Login from './components/Login';
import Signup from './components/Signup';
import Profile from './components/Profile';
import Create from './components/Create';
import Snippet from './components/Snippet';

const URL = process.env.REACT_APP_BACKEND_URL || process.env.REACT_APP_CUSTOM_BACKEND_URL || "http://localhost:8080";

const InitComponent = ({ user }) => {
    return user ? (
        <div className='container'>
            <h2 className='mt-5'>Welcome back!</h2>
            <Create />
        </div>
    ) : (
        <h3 className='text-center mt-5'>You need to <Link to="/login">login</Link></h3>
    );
};

function App() {
    const [user, setUser] = useState(false);
    const [loading, setLoading] = useState(true);
    
    useEffect(async () => {
        checkLoginStatus();
    }, []);

    const checkLoginStatus = async () => {
        try {
            const response = await axios.get(URL+"/user", { withCredentials: true });
            if (response.data) {
                setUser(response.data.user);
                setLoading(false);
            }
        } catch (error) {
            setLoading(false);
        }
    }

    if (loading) {
        return <div>Loading...</div>;
    }

    return (
        <Router>
            <Navbar isLoggedIn={user} />
            <Routes>
                <Route path='/' element={<InitComponent user={user} />} />
                <Route path='/login' element={<Login />} />
                <Route path='/signup' element={<Signup />} />
                <Route path='/profile' element={<Profile user={user} />} />
                <Route path='/create' element={<Create />} />
                <Route path='/snippet' element={<SnippetWrapper />} />
            </Routes>
        </Router>
    );
}

const SnippetWrapper = () => {
    const location = useLocation();
    const params = new URLSearchParams(location.search);
    const id = params.get('id');

    return <Snippet snippetId={id} hasBackButton={false} backToProfileFunction={() => {}} />;
};

export default App;
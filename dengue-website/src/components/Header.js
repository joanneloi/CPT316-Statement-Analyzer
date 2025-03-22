'use client';

import React, { useState } from 'react'
import './header.css';
import Nav from './Nav';
import Image from 'next/image';


export default function Header() {
  return (
    <header 
    id="header" 
    className="header d-flex align-items-center fixed-top"
    >
    <div className="container-fluid container-xl d-flex align-items-center justify-content-between">
      <div className="logo d-flex align-items-center">
          <Image src="/picture/logo.png" alt="Dengue Logo" width={40} height={40} />
        <h1>DENGUE</h1>
      </div>
        <Nav/>
    </div>
    </header>
  );
}

import React from 'react';
import { navs } from '@/data/data';
import './nav.css';
import Link from 'next/link';
import { usePathname } from 'next/navigation';

export default function Navigation() {
  const pathname = usePathname(); // Get current page URL

  return (
    <nav id="navbar" className="navbar">
      <ul>
        {navs.map((nav) => (
          <li key={nav.id} className={pathname === nav.link ? 'active' : ''}>
            <Link href={nav.link}>
              {nav.name === 'HOME' ? (
                <i className="bi bi-house-heart"></i>
              ) : (
                nav.name
              )}
            </Link>
          </li>
        ))}
      </ul>
    </nav>
  );
}

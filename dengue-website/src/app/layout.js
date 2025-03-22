import 'bootstrap-icons/font/bootstrap-icons.css';
import 'bootstrap/dist/css/bootstrap.css';
import 'aos/dist/aos.css';
import {EB_Garamond,Audiowide } from 'next/font/google';
import Header from '@/components/Header';
import "./globals.css";
import Footer from '@/sections/Footer';


const EB = EB_Garamond({ subsets: ["latin"], weight: "400" });
const audiowide = Audiowide({ subsets: ["latin"], weight: "400" });

export const metadata = {
  title: "Dengue Website",
  description: "A website about Dengue awareness",
};

export default function RootLayout({ children }) {
  return (
    <html lang="en">
      <body className={EB.className}>
        <Header fontClass={audiowide.className}/>
        {children}
        <Footer/>
      </body>
    </html>
  );
}
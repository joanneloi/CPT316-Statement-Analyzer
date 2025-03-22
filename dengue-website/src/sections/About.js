import React from 'react';
import Image from 'next/image';
import './about.css';

export default function About() {
  return (
    <section id="about" className="about">
      <div className="container" data-aos="fade-up">
        
        <div className="row align-items-center">
          {/* Left Side - Text Content */}
          <div className="col-lg-6 order-lg-1 text-center text-lg-start">
            <h2>About Dengue Awareness</h2>
            <p>
              Dengue fever is a mosquito-borne disease affecting millions worldwide. 
              Our goal is to spread awareness about its prevention, symptoms, and treatment.
            </p>
            <ul>
              <li>Understand the symptoms of Dengue</li>
              <li>Learn prevention methods to stay safe</li>
              <li>Spread awareness in your community</li>
            </ul>
          </div>

          {/* Right Side - Image */}
          <div className="col-lg-6 order-lg-2" data-aos="zoom-in" data-aos-delay="100">
            <div className="about-img">
              <Image src="/picture/aboutImage.png" alt="Dengue Awareness" width={500} height={400} />
            </div>
          </div>
        </div> {/* ✅ Closed first row properly */}


        {/* ✅ New Section Below */}
        <div className="row align-items-center mt-5">
          {/* Left Side - New Image */}
          <div className="col-lg-6 order-lg-1" data-aos="zoom-in" data-aos-delay="200">
            <div className="aboutimg">
              <Image src="/picture/aboutImage.png" alt="Dengue Prevention" width={500} height={400} />
            </div>
          </div>

          {/* Right Side - New Text Content */}
          <div className="col-lg-6 order-lg-2 text-center text-lg-start">
            <h2>Stay Protected from Dengue</h2>
            <p>
              Prevention is key to controlling Dengue outbreaks. Simple steps like eliminating mosquito breeding grounds,
              using insect repellents, and wearing protective clothing can significantly reduce the risk.
            </p>
            <ul>
              <li>Remove stagnant water around your home</li>
              <li>Use mosquito repellents and nets</li>
              <li>Participate in community cleanliness programs</li>
            </ul>
          </div>
        </div> 
      
      </div> 
    </section>
  );
}
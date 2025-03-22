import React from 'react';


export default function HeroSlide({slide}) {

  return (
    <a href="#" className="img-bg d-flex align-items-end"
        style={{backgroundImage:`url(${slide.bgImg})`}}
        >
            <div className="img-bg-inner">
                <h2>
                    {slide.title}
                </h2>
                <p>
                    {slide.brief}
                </p>
            </div>

    
    </a>
  );
}

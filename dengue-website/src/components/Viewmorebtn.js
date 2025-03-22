"use client"
import React from "react";
import "./viewmorebtn.css";

export default function Viewmorebtn({ name, target }) {
  const handleScrollTo = (id) => {
    const element = document.getElementById(id);
    if (element) {
      element.scrollIntoView({ behavior: "smooth" });
    }
  };

  return (
    <button className="view-more-btn" onClick={() => handleScrollTo(target)}>
        {name}
    </button>
  );
}
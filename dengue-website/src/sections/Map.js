import React from "react";
import "./map.css";

export default function Map() {
  return (
    <div className="map-container">
      <iframe
        className="map"
        src="https://www.google.com/maps/embed?pb=!1m18!1m12!1m3!1d16299639.609639848!2d109.61814849999999!3d4.1406339999999995!2m3!1f0!2f0!3f0!3m2!1i1024!2i768!4f13.1!3m3!1m2!1s0x3034d3975f6730af%3A0x745969328211cd8!2z6ams5p2l6KW_5Lqa!5e0!3m2!1szh-CN!2smy!4v1742577628682!5m2!1szh-CN!2smy"
        allowFullScreen
        loading="lazy"
      ></iframe>
    </div>
  );
}
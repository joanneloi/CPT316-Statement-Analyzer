import About from '@/sections/About';
import Hero from '@/sections/Hero';
import Viewmorebtn from '@/components/viewmorebtn';
import Image from "next/image";
import Map from '@/sections/Map';


export default function Home() {
  return (
    <>
   
    <section className="hero-background">
     
        <Image src={"/picture/BACKGROUND.png"} alt="Dengue Awareness" layout="fill" objectFit="cover" priority />

        <div className="button-container">
          <Viewmorebtn name="View More" target="about" />
        </div>
      </section>

     
      <Hero/>
     <main id="main">
    < About/>
    <Map/>
     </main>
</>
  );
}
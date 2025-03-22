
    import React from 'react';
    import './footer.css';
    
    export default function Footer() {
      return (
        <footer id="footer">
          <div className="footer-top">
            <div className="container">
              <div className="row">
                <div className="col-lg-3 col-md-6">
                  <div className="footer-info">
                    <h3>Dengue Website</h3>
                    <p>
                      By <br />
                      Yeexuan&Shuying <br /><br />
                      <strong>Phone:</strong> +60184608078 <br />
                      <strong>Email:</strong> abc@example.com <br />
                    </p>
                    <div className="social-links mt-3">
                      <a href="#" className="twitter"><i className="bi bi-twitter"></i></a>
                      <a href="#" className="facebook"><i className="bi bi-facebook"></i></a>
                      <a href="#" className="instagram"><i className="bi bi-instagram"></i></a>
                      <a href="#" className="linkedin"><i className="bi bi-linkedin"></i></a>
                    </div>
                  </div>
                </div>
              </div>
            </div>
          </div>
        </footer>
      );
    }

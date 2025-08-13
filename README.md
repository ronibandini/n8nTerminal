<img width="939" height="1230" alt="Flyern8nTerminal" src="https://github.com/user-attachments/assets/2ff1bf2f-2e14-4a4c-a626-7aaefac49fb2" />

# n8nTerminal
Physical buttons and screen for n8n 

# What's this

After releasing the Arduino library for adding a physical button to n8n https://www.hackster.io/roni-bandini/n8n-physical-button-ddfa0f , I noticed enough interest to take the idea one step further. That’s when I came up with a small, inexpensive ESP32-based device that not only adds physical buttons but can also receive information from n8n and display it on a screen.
Sure, you can start n8n workflows or handle “man in the loop” approvals with a computer or a phone app, but in some situations, having a dedicated device capable of handling buttons, sounds, sensors, camera, lights, and a screen can be much more practical.

Example use case: Imagine an e-commerce company where an employee currently uses a computer to check a Google Sheet to decide the next delivery. With this device, the employee simply presses a button. That triggers an n8n workflow, an AI agent connected to an LLM reads the Google Sheet, and selects the next delivery—not only based on order date but also considering customer notes (for example, prioritizing anxious customers). The AI agent then sends the label info directly to the device’s small screen, along with visual and audio cues. By pressing a second button, a QR code is displayed, ready to be scanned.
Sounds complicated? That’s exactly how my demo works—and it can easily be adapted for other scenarios and more complex projects.

# Parts 

Unihiker K10 https://www.dfrobot.com/product-2904.html?tracking=hOuIhw4fDaJRTdy4abz04npbQC78dqxBkqVt7XMFYxEXj2s0ukWgm71wbut0ewUP
microSD card https://www.dfrobot.com/product-1938.html?tracking=hOuIhw4fDaJRTdy4abz04npbQC78dqxBkqVt7XMFYxEXj2s0ukWgm71wbut0ewUP
2 Gravity digital buttons https://www.dfrobot.com/product-1097.html?tracking=hOuIhw4fDaJRTdy4abz04npbQC78dqxBkqVt7XMFYxEXj2s0ukWgm71wbut0ewUP

# Demo

https://www.youtube.com/shorts/4cWm9_oAfYg

# More information

https://bandini.medium.com/c%C3%B3mo-agregar-pantalla-y-botones-a-n8n-f7237ec4c5d5

# Contact

Roni Bandini https://www.linkedin.com/in/ronibandini/

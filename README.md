## C++ and DirectX 11 Aeroplane demo with Wiimote support
Using 'Tutorial 09 – Meshes using DXUT helper classes’ tutorial from Microsoft as a starting point, this demo has been heavily modified with an object orientated approach.

![Screenshot of demo in action](/screenshot.png)

#### Prerequisites 

* Download and install [DirectX 11 SDK](https://www.microsoft.com/en-us/download/details.aspx?id=6812)
* Project currently runs in [Visual Studio 2013 Community Edition](https://www.visualstudio.com/en-us/news/vs2013-community-vs.aspx)
* Build and run the program in Debug mode

#### WiiMote
* First somehow get the WiiMote connected to your computer
* Before the main demo is run - Open the folder named ‘WiimoteLib_1.8.src’, open the solution file and run the program
* Run the main demo, if all went to plan you can now take control of the aeroplane by holding down the A button.

#### Server
The beginnings of a server in the relevant directory, this barely works for two connected clients due to everything running in one main thread on the server. This could be improved in the future by adding multi threaded support.

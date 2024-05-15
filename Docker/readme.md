# Setup of Docker for MacOS

1. Download Docker for MacOS from [here](https://docs.docker.com/desktop/install/mac-install/)
2. Make sure you are in the project directory
3. Run the following command in your terminal:
    ```bash
      Docker build -t molsim -f Dockerfile .
    ```
4. Now you have an image called `molsim`, and you can check this by running the following command in your terminal:
   ```bash
   Docker images
   ```
5. Now you can either choose to use this image to build the project using terminal and use gdb for debugging or use the 
in-build Clion toolchain. I'd recommend setting up both
   - Setting up the terminal way
     
     - Use the following command in your terminal, and then you'll have the same experience as if you had native Ubuntu.
        ```bash
        docker run -it --rm -v `pwd`:/work molsim
        ```
     - Now you should notice your terminal changes to something like `root@text-here:/work`,
       and now you can follow the `readme.md` in 
       the project dir for further instructions on the build process. 
     - To quit and return to your macOS terminal, use the following command:
       ```bash
       exit
       ```
   - Setting up Toolchain for Clion
     - Go to your Clion settings and you'll see **Build, Execution, Deployment**. 
     - Inside this you'll see **Toolchains**
     - You'll need to create a new one using the **+** icon and choosing **Docker** as the option.
     - You can name it whatever you want, just make sure that the following settings match:
       - Server: Docker (You'll see a settings clog, click on it and make sure that **Docker For Mac** and **Default** are selected)
       - image: molsim:latest
       - Container Settings: --entrypoint --rm
       - Cmake: Docker CMake
       - ALL OTHER SHOULD BE DEFAULTS AND AUTO DETECTED
       - Debugger: Docker GDB
       
       Now make this toolchain as **default** by selecting the Docker toolchain and click the up icon. (If you do this, skip the below steps.)
     - Now you need to **CMake** that should be right below **Toolchains**.
       - You need to make a new Profile for CMake.
       - Copy your existing one and change the **Toolchain** setting to Docker.
       - Now you can either make this default using the up icon or select it using the scroll-down menu next to the Run and Debug buttons on top.
       - That's it.
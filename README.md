# UART-Calculator

A utility designed for the TIVA C series microcontroller, this calculator assists in configuring UART settings such as baud rate, parity, and stop bits, aiding in the development and debugging of serial communication interfaces.

## Features

- Baud Rate Calculation: Compute the optimal baud rate settings based on the desired communication parameters.
- Parity and Stop Bit Configuration: Determine the appropriate settings for parity and stop bits to ensure reliable data transmission.
- Error Detection: Identify potential configuration errors that could lead to communication issues.
- User-Friendly Interface: Simple command-line interface for easy interaction.

## Technologies Used

- C Programming Language: The primary language used for development.
- TIVA C Series Microcontroller: Specifically designed for the TM4C123GH6PM microcontroller.
- Code Composer Studio: Integrated development environment used for coding and debugging.

## Setup Instructions

1. Clone the Repository:

   ```
   git clone https://github.com/seifddin02/UART-Calculator.git
   cd UART-Calculator
   ```

2. Open the Project:

   - Launch Code Composer Studio.
   - Open the `UART-Calculator` project directory.

3. Build the Project:

   - In Code Composer Studio, build the project to compile the source code.

4. Load the Program:

   - Connect the TIVA C series microcontroller to your computer.
   - Load the compiled program onto the microcontroller.

5. Run the Program:

   - Start the program execution.
   - Follow the on-screen prompts to configure UART settings.

## Usage

- Input the desired baud rate, parity, and stop bits when prompted.
- The calculator will display the optimal settings and any potential errors.
- Use the information to configure your microcontroller's UART module accordingly.

## Contributing

Contributions are welcome! To contribute:

1. Fork the repository.
2. Create a new branch (`git checkout -b feature-name`).
3. Make your changes.
4. Commit your changes (`git commit -am 'Add new feature'`).
5. Push to the branch (`git push origin feature-name`).
6. Create a new Pull Request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Texas Instruments: For providing the TIVA C series microcontroller and associated resources.
- Code Composer Studio: For offering a comprehensive development environment for embedded systems.
- Open Source Community: For their contributions and support in embedded systems development.

# QwiicUART Library
This is an Arduino library to utilize the NXP SC16IS741 as a hardware serial port controlled via I<sup>2</sup>C.



## QwiicUART Board



## Examples



## Methods



<details><summary id="QwiicUART-1"><strong>QwiicUART()</strong></summary>
  <blockquote>

### Description
Creates a QwiicUART object and sets the I<sup>2</sup>C port and address to use.

### Syntax
- `QwiicUART`
- `QwiicUART(wire)`
- `QwiicUART(wire, address)`

### Parameters
- `wire`: the `TwoWire` object to use to talk to the UART chip. The default value is `Wire`.
- `address`: The I<sup>2</sup>C address of the UART chip. Typical values are:  
  `0x48` (default)  
  `0x49`  
  `0x4A`  
  `0x4B`  

### Example
``` C++
# include <QwiicUART.h>

QwiicUART qSerial(Wire, 0x48);
```

  </blockquote>
</details>



<details><summary id="available"><strong>available()</strong></summary>
  <blockquote>

### Description
Get the number of bytes (characters) available for reading from the serial port. This is data that’s already arrived and stored in the serial receive buffer (which holds 64 bytes).

`available()` inherits from the [Stream](https://docs.arduino.cc/language-reference/en/functions/communication/stream/) utility class.

### Syntax
`qSerial.available()`

### Parameters
`qSerial`: a `QwiicUART` serial port object.

### Returns
The number of bytes available to read.

### Example
``` C++
# include <QwiicUART.h>

QwiicUART qSerial;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  qSerial.begin(9600);
}

void loop() {
  // read from Serial, send to qSerial:
  if (Serial.available()) {
    int inByte = Serial.read();
    qSerial.print(inByte, DEC);
  }
  // read from qSerial, send to Serial:
  if (qSerial.available()) {
    int inByte = qSerial.read();
    Serial.print(inByte, DEC);
  }
}
```

  </blockquote>
</details>



<details><summary id="availableForWrite"><strong>availableForWrite()</strong></summary>
  <blockquote>

### Description
Get the number of bytes (characters) available for writing in the serial buffer without blocking the write operation.

### Syntax
`qSerial.availableForWrite()`

### Parameters
`qSerial`: a `QwiicUART` serial port object.

### Returns
The number of bytes available to write.

  </blockquote>
</details>



<details><summary id="begin"><strong>begin()</strong></summary>
  <blockquote>

### Description
Sets the data rate in bits per second (baud) for serial data transmission.

An optional second argument configures the data, parity, and stop bits. The default is 8 data bits, no parity, one stop bit.

### Syntax
- `qSerial.begin(baud)`
- `qSerial.begin(baud, config)`

### Parameters
- `qSerial`: a `QwiicUART` serial port object.
- `baud`: in bits per second.
  Common values are: `1200`, `2400`, `4800`, `9600`, `16200`, `38400`, `57600`, and `115200`.
  Allowed data types: `long`.
- `config`: sets data, parity, and stop bits. Valid values are:
  - `SERIAL_5N1`  
  - `SERIAL_6N1`  
  - `SERIAL_7N1`  
  - `SERIAL_8N1` (default)  
  - `SERIAL_5N2`  
  - `SERIAL_6N2`  
  - `SERIAL_7N2`  
  - `SERIAL_8N2`  
  - `SERIAL_5E1`: even parity  
  - `SERIAL_6E1`  
  - `SERIAL_7E1`  
  - `SERIAL_8E1`  
  - `SERIAL_5E2`  
  - `SERIAL_6E2`  
  - `SERIAL_7E2`  
  - `SERIAL_8E2`  
  - `SERIAL_5O1`: odd parity  
  - `SERIAL_6O1`  
  - `SERIAL_7O1`  
  - `SERIAL_8O1`  
  - `SERIAL_5O2`  
  - `SERIAL_6O2`  
  - `SERIAL_7O2`  
  - `SERIAL_8O2`

### Example
``` C++
# include <QwiicUART.h>

QwiicUART qSerial;

void setup() {
  Wire.begin();
  qSerial.begin(9600);
}

void loop() {}
```

> [!NOTE]
> The actual baud rate of the SC16IS741 is set using an integer $`divisor`$.
> When using a 1.8432 MHz clock, as the [QwiicUART board](https://github.com/CMB27/QwiicUART-Board) does, the baud rate will be:
> $$\frac{115200}{divisor}$$
> This library selects the $`divisor`$ value that will result in the baud rate closest to what is asked for, but because it is an integer, there is limited resolution, particularly at the higher baud rates.
> | $`divisor`$ | Baud Rate |
> | ----------: | --------: |
> |           1 |    115200 |
> |           2 |     57600 |
> |           3 |     38400 |
> |           4 |     28800 |
> |           5 |     23040 |
> |           6 |     19200 |

  </blockquote>
</details>



<details><summary id="end"><strong>end()</strong></summary>
  <blockquote>

### Description
Runs flush() then resets the SC16IS741.

### Syntax
`qSerial.end()`

### Parameters
`qSerial`: a `QwiicUART` serial port object.

  </blockquote>
</details>



<details><summary id="find"><strong>find()</strong></summary>
  <blockquote>

### Description
Reads data from the serial buffer until the target is found. The function returns `true` if target is found, `false` if it times out.

`find()` inherits from the [Stream](https://docs.arduino.cc/language-reference/en/functions/communication/stream/) utility class.

### Syntax
- `qSerial.find(target)`
- `qSerial.find(target, length)`

### Parameters
- `qSerial`: a `QwiicUART` serial port object.
- `target`: the string to search for. Allowed data types: `char`.
- `length`: length of the target. Allowed data types: `size_t`.

### Returns
Data type: `bool`.

  </blockquote>
</details>



<details><summary id="findUntil"><strong>findUntil()</strong></summary>
  <blockquote>

### Description
Reads data from the serial buffer until a target string of given length or terminator string is found.

The function returns `true` if the target string is found, `false` if it times out.

`findUntil()` inherits from the [Stream](https://docs.arduino.cc/language-reference/en/functions/communication/stream/) utility class.

### Syntax
`qSerial.findUntil(target, terminal)`

### Parameters
- `qSerial`: a `QwiicUART` serial port object.
- `target`: the string to search for. Allowed data types: `char`.
- `terminal`: the terminal string in the search. Allowed data types: `char`.

### Returns
Data type: `bool`.

  </blockquote>
</details>



<details><summary id="flush"><strong>flush()</strong></summary>
  <blockquote>

### Description
Waits for the transmission of outgoing serial data to complete.

`flush()` inherits from the [Stream](https://docs.arduino.cc/language-reference/en/functions/communication/stream/) utility class.

### Syntax
`qSerial.flush()`

### Parameters
`qSerial`: a `QwiicUART` serial port object.

  </blockquote>
</details>



<details><summary id="parseFloat"><strong>parseFloat()</strong></summary>
  <blockquote>

### Description
Returns the first valid floating point number from the Serial buffer.
`parseFloat()` is terminated by the first character that is not a floating point number.
The function terminates if it times out (see `setTimeout()`).

`parseFloat()` inherits from the [Stream](https://docs.arduino.cc/language-reference/en/functions/communication/stream/) utility class.

### Syntax
- `qSerial.parseFloat()`
- `qSerial.parseFloat(lookahead)`
- `qSerial.parseFloat(lookahead, ignore)`

### Parameters
- `qSerial`: a `QwiicUART` serial port object.
- `lookahead`: the mode used to look ahead in the stream for a floating point number. Allowed data types: `LookaheadMode`. Allowed values:
  - `SKIP_ALL`: All characters other than a minus sign, decimal point, or digits are ignored when scanning the stream for a floating point number. This is the default mode.
  - `SKIP_NONE`: Nothing is skipped, and the stream is not touched unless the first waiting character is valid.
  - `SKIP_WHITESPACE`: Only tabs, spaces, line feeds, and carriage returns are skipped.
- `ignore`: used to skip the indicated char in the search. Used for example to skip thousands divider. Allowed data types: `char`

### Returns
Data type: `float`.

  </blockquote>
</details>



<details><summary id="parseInt"><strong>parseInt()</strong></summary>
  <blockquote>

### Description
Looks for the next valid integer in the incoming serial. The function terminates if it times out (see `setTimeout()`).

`parseInt()` inherits from the [Stream](https://docs.arduino.cc/language-reference/en/functions/communication/stream/) utility class.

In particular:

- Parsing stops when no characters have been read for a configurable time-out value, or a non-digit is read;
- If no valid digits were read when the time-out (see `setTimeout()`) occurs, 0 is returned;

### Syntax
- `qSerial.parseInt()`
- `qSerial.parseInt(lookahead)`
- `qSerial.parseInt(lookahead, ignore)`

### Parameters
- `qSerial`: a `QwiicUART` serial port object.
- `lookahead`: the mode used to look ahead in the stream for an integer. Allowed data types: `LookaheadMode`. Allowed values:
  - `SKIP_ALL`: All characters other than digits or a minus sign are ignored when scanning the stream for an integer. This is the default mode.
  - `SKIP_NONE`: Nothing is skipped, and the stream is not touched unless the first waiting character is valid.
  - `SKIP_WHITESPACE`: Only tabs, spaces, line feeds, and carriage returns are skipped.
- `ignore`: used to skip the indicated char in the search. Used for example to skip thousands divider. Allowed data types: `char`

### Returns
The next valid integer. Data type: `long`.

  </blockquote>
</details>



<details><summary id="peek"><strong>peek()</strong></summary>
  <blockquote>

### Description
Returns the next byte (character) of incoming serial data without removing it from the internal serial buffer.
That is, successive calls to `peek()` will return the same character, as will the next call to `read()`.

`peek()` inherits from the [Stream](https://docs.arduino.cc/language-reference/en/functions/communication/stream/) utility class.

### Syntax
`qSerial.peek()`

### Parameters
`qSerial`: a `QwiicUART` serial port object.

### Returns
The first byte of incoming serial data available (or -1 if no data is available). Data type: `int`.

  </blockquote>
</details>



<details><summary id="print"><strong>print()</strong></summary>
  <blockquote>

### Description
Prints data to the serial port as human-readable ASCII text.
This command can take many forms. Numbers are printed using an ASCII character for each digit.
Floats are similarly printed as ASCII digits, defaulting to two decimal places.
Bytes are sent as a single character.
Characters and strings are sent as is.
For example-

- `qSerial.print(78)` gives "78"
- `qSerial.print(1.23456)` gives "1.23"
- `qSerial.print('N')` gives "N"
- `qSerial.print("Hello world.")` gives "Hello world."

An optional second parameter specifies the base (format) to use; permitted values are `BIN(binary, or base 2)`, `OCT(octal, or base 8)`, `DEC(decimal, or base 10)`, `HEX(hexadecimal, or base 16)`.
For floating point numbers, this parameter specifies the number of decimal places to use.
For example:

- `qSerial.print(78, BIN)` gives "1001110"
- `qSerial.print(78, OCT)` gives "116"
- `qSerial.print(78, DEC)` gives "78"
- `qSerial.print(78, HEX)` gives "4E"
- `qSerial.print(1.23456, 0)` gives "1"
- `qSerial.print(1.23456, 2)` gives "1.23"
- `qSerial.print(1.23456, 4)` gives "1.2346"

To send data without conversion to its representation as characters, use `write()`.

### Syntax
- `qSerial.print(value)`
- `qSerial.print(value, format)`

### Parameters
- `qSerial`: a `QwiicUART` serial port object.
- `value`: the value to print. Allowed data types: any data type.
- `format`: specifies the number base (for integral data types) or number of decimal places (for floating point types).

### Returns
The number of bytes written, though reading that number is optional. Data type: `size_t`.

### Example
``` C++
/*
  Uses a for loop to print numbers in various formats.
*/

# include <QwiicUART.h>

QwiicUART qSerial;

void setup() {
  Wire.begin();
  qSerial.begin(9600);
}

void loop() {
  // print labels
  qSerial.print("NO FORMAT");  // prints a label
  qSerial.print("\t");         // prints a tab

  qSerial.print("DEC");
  qSerial.print("\t");

  qSerial.print("HEX");
  qSerial.print("\t");

  qSerial.print("OCT");
  qSerial.print("\t");

  qSerial.print("BIN");
  qSerial.println();        // carriage return after the last label

  for (int x = 0; x < 64; x++) { // only part of the ASCII chart, change to suit
    // print it out in many formats:
    qSerial.print(x);       // print as an ASCII-encoded decimal - same as "DEC"
    qSerial.print("\t\t");  // prints two tabs to accomodate the label length

    qSerial.print(x, DEC);  // print as an ASCII-encoded decimal
    qSerial.print("\t");    // prints a tab

    qSerial.print(x, HEX);  // print as an ASCII-encoded hexadecimal
    qSerial.print("\t");    // prints a tab

    qSerial.print(x, OCT);  // print as an ASCII-encoded octal
    qSerial.print("\t");    // prints a tab

    qSerial.println(x, BIN);  // print as an ASCII-encoded binary
    // then adds the carriage return with "println"
    delay(200);            // delay 200 milliseconds
  }
  qSerial.println();        // prints another carriage return
}
```

> [!NOTE]
> QwiicUART "Serial" transmission is asynchronous.
> Depending on the baud rate and I<sup>2</sup>C clock, `print()` may return before all the characters are transmitted.
> If the transmit buffer/FIFO is full then `print()` will block until there is enough space in the buffer.
> To avoid blocking calls to `print()`, you can first check the amount of free space in the transmit buffer using `availableForWrite()`.

  </blockquote>
</details>



<details><summary id="println"><strong>println()</strong></summary>
  <blockquote>

### Description
Prints data to the serial port as human-readable ASCII text followed by a carriage return character (ASCII 13, or '\r') and a newline character (ASCII 10, or '\n').
This command takes the same forms as `print()`.

### Syntax
- `qSerial.println(value)`
- `qSerial.println(value, format)`

### Parameters
- `qSerial`: a `QwiicUART` serial port object.
- `value`: the value to print. Allowed data types: any data type.
- `format`: specifies the number base (for integral data types) or number of decimal places (for floating point types).

### Returns
The number of bytes written, though reading that number is optional. Data type: `size_t`.

### Example
``` C++
/*
  Analog input reads an analog input on analog in 0, prints the value out.
  Created: 2026-03-24
  By: Tom Igoe
  Adapted for QwiicUART on: 2025-02-12
  By: C. M. Bulliner
*/

# include <QwiicUART.h>

QwiicUART qSerial;

int analogValue = 0;    // variable to hold the analog value

void setup() {
  Wire.begin();
  qSerial.begin(9600);
}

void loop() {
  // read the analog input on pin 0:
  analogValue = analogRead(0);

  // print it out in many formats:
  qSerial.println(analogValue);       // print as an ASCII-encoded decimal
  qSerial.println(analogValue, DEC);  // print as an ASCII-encoded decimal
  qSerial.println(analogValue, HEX);  // print as an ASCII-encoded hexadecimal
  qSerial.println(analogValue, OCT);  // print as an ASCII-encoded octal
  qSerial.println(analogValue, BIN);  // print as an ASCII-encoded binary

  // delay 10 milliseconds before the next reading:
  delay(10);
}
```

> [!NOTE]
> QwiicUART "Serial" transmission is asynchronous.
> Depending on the baud rate and I<sup>2</sup>C clock, `println()` may return before all the characters are transmitted.
> If the transmit buffer/FIFO is full then `println()` will block until there is enough space in the buffer.
> To avoid blocking calls to `println()`, you can first check the amount of free space in the transmit buffer using `availableForWrite()`.

  </blockquote>
</details>



<details><summary id="read"><strong>read()</strong></summary>
  <blockquote>

### Description
Reads incoming serial data.

`read()` inherits from the [Stream](https://docs.arduino.cc/language-reference/en/functions/communication/stream/) utility class.

### Syntax
`qSerial.read()`

### Parameters
`qSerial`: a `QwiicUART` serial port object.

### Returns
The first byte of incoming serial data available (or -1 if no data is available). Data type: `int`.

### Example
``` C++
# include <QwiicUART.h>

QwiicUART qSerial;

int incomingByte = 0; // for incoming serial data

void setup() {
  Serial.begin(9600);
  Wire.begin();
  qSerial.begin(9600);
}

void loop() {
  // send data only when you receive data:
  if (qSerial.available() > 0) {
    // read the incoming byte:
    incomingByte = qSerial.read();

    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);
  }
}
```

  </blockquote>
</details>













## Legal Stuff
Much of the documentation in this README was shamelessly copied from the [Arduino Docs Language Reference](https://docs.arduino.cc/language-reference/), particularly from the [Serial](https://docs.arduino.cc/language-reference/en/functions/communication/serial/) section.  
This is legal, provided:
- I give Arduino apprpriate credit, which I think I just did
- Note if any changes were made, which there were
- And if I distribute this README that it be under the same license as the Arduino Documentation, which is the [Creative Commons Attribution Share Alike 4.0 license](https://creativecommons.org/licenses/by-sa/4.0/)

Therefore, even though the source code of this library is distributed under the [MIT license](LICENSE), this README is distributed under the [Creative Commons Attribution Share Alike 4.0 license](https://creativecommons.org/licenses/by-sa/4.0/).

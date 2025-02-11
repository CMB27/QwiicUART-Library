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
  - `SKIP_ALL`: all characters other than a minus sign, decimal point, or digits are ignored when scanning the stream for a floating point number. This is the default mode.
  - `SKIP_NONE`: Nothing is skipped, and the stream is not touched unless the first waiting character is valid.
  - `SKIP_WHITESPACE`: Only tabs, spaces, line feeds, and carriage returns are skipped.
- `ignore`: used to skip the indicated char in the search. Used for example to skip thousands divider. Allowed data types: `char`

### Returns
Data type: `float`.

  </blockquote>
</details>

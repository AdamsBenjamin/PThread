ostream& operator<<(ostream& os, const Date& dt)
{
    os << dt.mo << '/' << dt.da << '/' << dt.yr;
    return os;
}# PThreader

## What is this?
A class-based abstraction of the pthread library.<br>
Inspired by the design of Java's Thread.

## Dependencies
g++ 8.3.0<br>
googletest 1.8.1-3

## Building
Run `make build` from the root directory.

## Running
`./pthreader`

## Testing
-- *In the works*

use std::net::{TcpStream};
use std::io::{Read, Write};
use std::str::from_utf8;

fn main()
{
    match TcpStream::connect("localhost:8080")
    {
        Ok(mut stream) =>
            {
                println!("Connected 8080");
                let message = b"Hello!";
                
                stream.write(message).expect("Error at sending");
                println!("Sent Hello, awaiting for reply...");
                
                let mut data = [0u8; 6];
                match stream.read_exact(&mut data)
                {
                    Ok(_) =>
                        {
                            if &data == message
                            {
                                println!("Reply is correct!");
                            }
                            else
                            {
                                let text = from_utf8(&data).expect("Parsing is failed");
                                println!("Unexpected reply: {}", text);
                            }
                        }
                    Err(e) =>
                        {
                            println!("Failed to receive data: {}", e);
                        }
                }
                
                println!("Process finished");
            }
        Err(e) =>
            {
                println!("Failed to connect: {}", e);
            }
    }
    return;
}
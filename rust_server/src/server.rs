use std::thread;
use std::net::{TcpListener, TcpStream, Shutdown};
use std::io::{Read, Write};

fn handle_client(mut tcp_stream: TcpStream)
{
    let mut buffer = [0u8; 50];
    
    while match tcp_stream.read(&mut buffer)
    {
        Ok(size) =>
            {
                tcp_stream.write(&buffer[0 .. size]).unwrap();
                true
            },
        Err(_) =>
            {
                println!("An error while handling the client: {}", tcp_stream.peer_addr().unwrap());
                tcp_stream.shutdown(Shutdown::Both).expect("Error in shutting down");
                false
            }
    } {}
}

fn main()
{
    let listener = TcpListener::bind("0.0.0.0:8080").expect("Error while binding");
    println!("Server is listening on port {}", 8080);
    
    for stream in listener.incoming()
    {
        match stream
        {
            Ok(stream) =>
                {
                    println!("New connection: {}", stream.peer_addr().expect("Error at new connection"));
                    thread::spawn(move || { handle_client(stream) });
                }
            Err(e) =>
                {
                    println!("Error: {}", e);
                    // TODO - handle connection failed
                }
        }
    }
    drop(listener);
    return;
}
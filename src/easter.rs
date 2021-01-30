use notify::{watcher, RecursiveMode, Watcher};
use std::env;
use std::fs;
use std::sync::mpsc;
use std::sync::{Arc, Mutex};
use std::thread;
use std::time::Duration;

fn main() {
    let args: Vec<String> = env::args().collect();
    assert_eq!(args.len(), 2);

    let path = Arc::new(String::from(&args[1]));
    let content = Arc::new(Mutex::new(fs::read_to_string(&path[..]).unwrap()));

    let path_clone = path.clone();
    let content_clone = content.clone();
    thread::spawn(move || {
        let (tx, rx) = mpsc::channel();
        let mut watcher = watcher(tx, Duration::from_secs(1)).unwrap();
        watcher
            .watch(&path_clone[..], RecursiveMode::NonRecursive)
            .unwrap();
        loop {
            match rx.recv() {
                Ok(_e) => {
                    let mut content = content_clone.lock().unwrap();
                    content.replace_range(.., &fs::read_to_string(&path_clone[..]).unwrap()[..]);
                }
                Err(e) => panic!("watch error: {:?}", e),
            }
        }
    });

    loop {
        println!("{}", content.lock().unwrap());
        thread::sleep(Duration::from_secs(1));
    }
}

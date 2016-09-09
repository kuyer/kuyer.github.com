package main

import (
	"fmt"
	"log"
	"net/http"
)

func main() {
	http.HandleFunc("/hello", handleHello)
	fmt.Println("server start on http://localhost:8888/hello");
	log.Fatal(http.ListenAndServe("localhost:8888", nil));
}

func handleHello(w http.ResponseWriter, req *http.Request) {
	log.Println("serving", req.URL);
	fmt.Fprintln(w, "hello, kuyer!");
}

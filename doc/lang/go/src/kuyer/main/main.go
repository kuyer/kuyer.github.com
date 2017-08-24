package main

import (
	"fmt"
	"kuyer/math"
)

func main() {
	fmt.Printf("hello, go!\n");
	pipe := make(chan int, 1)
	go math.Add(pipe, 10, 20)
	sum := <-pipe
	fmt.Println(sum)
}

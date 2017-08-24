package math

func Add(pipe chan int, a int, b int) {
	pipe <- (a+b)
}

from multiprocessing import Process, Queue
import SpreadsheedAPI as SpreadApi
import multiprocessing

def square(numbers, queue):
	for i in numbers:
		queue.put(f'{i*i},x,y,z')

def cube(numbers, queue):
	for i in numbers:
		queue.put(f'{i*i*i},x,y,z')


if __name__ == '__main__':
	numbers = range(5)
	queue = Queue()

	square_prosess = Process(target=square, args=(numbers, queue))
	cube_prosess = Process(target=cube, args=(numbers, queue))

	square_prosess.start()
	cube_prosess.start()

	square_prosess.join()
	cube_prosess.join()

	unpacked_queue = ""
	timestamped = []
	while not queue.empty():
		#unpacked_queue = (queue.get())
		timestamped.append(SpreadApi.formatter(queue.get()))
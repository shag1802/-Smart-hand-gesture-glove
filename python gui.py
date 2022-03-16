"""
Gui for electronics project 

"""
import serial 
import pickle
import pygame
import sys
import pyttsx3
import time
import threading
from pygame import mixer




pygame.mixer.init()

Songs = ["songs/1.mp3","songs/2.mp3"]


engine = pyttsx3.init()



def Text_to_speech(text):
	engine = pyttsx3.init()
	engine.say(text)
	#print("something")
	
	

def Setup(): 
	pygame.init()
	screen = pygame.display.set_mode((600,600))
	pygame.display.set_caption('EC Lab Project')
	Background = pygame.image.load('something.jpg')
	font = pygame.font.Font('freesansbold.ttf', 32)
	return [Background,screen,font]


def main():
	curr_song = 0
	back_ground,screen,font = Setup()
	count = 0
	num_prev = 0
	text = 'default </mode will be shown here>'
	
	
	aurdinodata = serial.Serial('/dev/cu.usbmodem14201',9600)
	value = 0
	int_val = 0
	while True:
		screen.fill((211,211,211))
		pygame.draw.line(screen,(0,0,0),(0,0),(0,600),30)
		pygame.draw.line(screen,(0,0,0),(0,600),(600,600),30)
		pygame.draw.line(screen,(0,0,0),(0,0),(600,0),30)
		pygame.draw.line(screen,(0,0,0),(600,0),(600,600),30)

		if(aurdinodata.in_waiting):
			value = aurdinodata.read()
			int_val = int.from_bytes(value,"little")
			print(int_val)
			print("sdetected")



		if(int_val == 6):
			mixer.music.load(Songs[curr_song])
			mixer.music.play()
			mixer.music.set_volume(1)


		if(int_val == 8):
			if(curr_song == 1):
				curr_song = 0


			else:
				curr_song = 1


			mixer.music.load(Songs[curr_song])
			mixer.music.play()
			mixer.music.set_volume(1)



		if(int_val == 7):
			mixer.music.pause()


		if(int_val == 9):
			if(curr_song == 0):
				curr_song = 1


			else:
				curr_song = 0


			mixer.music.load(Songs[curr_song])
			mixer.music.play()
			mixer.music.set_volume(1)



		if(int_val == 235):
			mixer.music.set_volume(1)


		if(int_val == 234):
			mixer.music.set_volume(0.7)


		if(int_val == 233):
			mixer.music.set_volume(0.3)



		if(int_val == 0):
			for i in Render_text(0):
				screen.blit(i[0],i[1])


		else:
			screen.blit(Render_text(int_val)[0],Render_text(int_val)[1])
		
		for e in pygame.event.get():
			if(e.type == pygame.QUIT):
				pygame.quit()
				sys.exit()



		count += 1
		pygame.display.update()



def increment(a):
	a=a+1
	return a
def Render_text(number):
	flag = 0
	back_ground,screen,font = Setup()
	text_list = []
	if(number == 0):
		text_list.append(return_text("Menu",40))
		text_list.append(return_text("1. Menu Mode",100))
		text_list.append(return_text("2. Light",140))
		text_list.append(return_text("3. Fan",180))
		text_list.append(return_text("4. Songs",220))
		text_list.append(return_text("Welcome User,",260))
		text_list.append(return_text("There are four modes in our project",300))
		text_list.append(return_text("To change the modes bend your index",340))
		text_list.append(return_text("and middle finger at the same time",380))
		text_list.append(return_text("By default first finger is usedto some",420))
		text_list.append(return_text("default gestures",460))
		text_list.append(return_text("The user can also program the ",500))
		text_list.append(return_text("gestures,By entering into gesturing",540))
		text_list.append(return_text("changing mode",580))

		
		return text_list


	elif(number == 2):
		return return_text("light on",300)


	elif(number == 3):
		return return_text("light off",300)


	elif(number == 11):
		return return_text("Input registered for the light mode",300)


	elif(number == 10):
		return return_text("Now register your Input for light mode",300)


	elif(number == 4):
		return return_text("fan on",300)


	elif(number == 5):
		return return_text("fan off",300)


	elif(number == 245):
		return return_text("fan on(Full speed)",300)


	elif(number == 244):
		return return_text("fan on 70%(speed)",300)

	elif(number == 243):
		return return_text("fan on 35%(speed)",300)


	elif(number == 20):
		return return_text("Register input for fan mode now",300)


	elif(number == 21):
		return return_text("Input registered for fan mode",300)


	elif(number == 6):
		return return_text("play",300)


	elif(number == 7):
		return return_text("pause",300)


	elif(number == 8):
		return return_text("Forward",300)


	elif(number == 9):
		return return_text("backword",300)


	elif(number == 30):
		return return_text("Register your input for play and pause",300)


	elif(number == 31):
		return return_text("input registered for play and pause",300)


	elif(number == 60):
		return return_text("Gesture changing mode",300)


	elif(number == 51):
		return return_text("Finger registered is first",300)

	elif(number == 52):
		return return_text("Finger registered is Second",300)


	elif(number == 53):
		return return_text("Finger registered is Third",300)


	elif(number == 40):
		return return_text("Rigister your input for forword",300)


	elif(number == 41):
		return return_text("Input registered for forword",300)


	elif(number == 70):
		return return_text("Register your input for backward",300)


	elif(number == 71):
		return return_text("input registered for backward",300)


	elif(number == 15):
		return return_text("Mode 0",300)


	elif(number == 16):
		return return_text("Mode 1",300)


	elif(number == 17):
		return return_text("Mode 2",300)


	elif(number == 18):
		return return_text("Mode 3",300)


	elif(number == 235):
		return return_text("Full volume",300)


	elif(number == 234):

		return return_text("60% volume",300)


	elif(number == 233):
		return return_text("30% volume",300)


	elif(number == 255):
		return return_text("Led on(Full Brightness)",300)


	elif(number == 254):
		return return_text("Led on(70% Brightness)",300)


	elif(number == 253):
		return return_text("Led on(35% Brightness)",300)


	elif(number == 88):
		return return_text("Gyro open",300)


	elif(number == 89):
		return return_text("Gyro close",300)


	else:
		return return_text("None mode(",300)


def return_text(name,y_pos):
	font = pygame.font.Font('freesansbold.ttf', 32)
	text = font.render(name,True,(0,0,128),(211,211,211))
	textRect = text.get_rect()
	textRect.center = (300,y_pos)

	return [text,textRect]

if __name__ == "__main__":
	main()
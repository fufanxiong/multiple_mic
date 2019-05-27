/*
	rtl8711bn set es8155 by iic

	flow
	1、when rtl8711bn set up
	2、set es8155 default and read register
	3、when get mp3 format(rtl8711bn this handle only can resolve no header and ender mp3 -- this from baidu tts)
	4、set es8155 register
	5、enable DAC
	6、send mp3 format data by i2s
	7、disable DAC

*/

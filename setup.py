from distutils.core import setup, Extension

module1 = Extension(
	'solver', # название модуля внутри Python
	 sources = ['setup.c'] # Исходные файлы модуля
)

setup(
	name = 'solver',              # название модуля (solver.__name__)
	version = '1.1',               # версия модуля
	description = 'Simple module', # описание модуля
	ext_modules = [module1]         # объекты типа Extension (мы объявили его выше)
)
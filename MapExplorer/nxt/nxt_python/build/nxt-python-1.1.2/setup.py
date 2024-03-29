#!/usr/bin/env python

from distutils.core import setup

readme = open('README', 'r')
ldesc = readme.read(8192)
readme.close()

setup(
	name='nxt-python',
	version='1.1.2',
	author='Douglas Lau, Marcus Wanner',
	author_email='dplau@comcast.net, marcusw@cox.net',
	description='LEGO Mindstorms NXT Control Package',
        url='http://home.comcast.net/~dplau/nxt_python/, http://code.google.com/p/nxt-python/',
        license='Gnu GPL v2',
	packages=['nxt'],
	scripts=['scripts/nxt_push', 'scripts/nxt_test', 'scripts/nxt_filer'],
        long_description=ldesc
)

import curses

STR = '''
def start(self):
    key = 0
    page_size = len(self.presentation_content)
    current_page = -1
'''

def main():

	stdscr = curses.initscr()
	curses.start_color()

	curses.noecho()
	curses.cbreak()
	stdscr.keypad(True)

	stdscr.addstr(3, 0, STR)
	stdscr.refresh()
	stdscr.getkey()

	return 0

if __name__ == '__main__':
	STATUS = main()
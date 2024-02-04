from pathlib import Path

everything = list(Path('api/logs').iterdir())

for something in everything:
    if something.name != 'clear.py':
        something.unlink(missing_ok = True)
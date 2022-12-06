import mido
import pygame
import pygame.gfxdraw
import threading

global_time = 0

COLORS = [
    (35, 145, 99),
    (234, 185, 4),
    (64, 182, 224),
    (49, 105, 56),
    (124, 103, 213),
    (255, 231, 146),
    (232, 66, 52)
]


def play_midi_file(midi_file: mido.MidiFile, stop_event):
    global global_time

    port = mido.open_output()

    for message in midi_file.play():
        port.send(message)

        global_time += message.time

        if stop_event.is_set():
            break


class ParsedNote:
    def __init__(
        self, value: int,
        velocity: int,
        channel: int,
        trigger_time: float,
        play_time: float = 0.0
    ):
        self._value = value
        self._velocity = velocity
        self._channel = channel
        self._trigger_time = trigger_time
        self._play_time = play_time

    @property
    def value(self):
        return self._value

    @property
    def velocity(self):
        return self._velocity

    @property
    def channel(self):
        return self._channel

    @property
    def trigger_time(self):
        return self._trigger_time

    @property
    def play_time(self):
        return self._play_time

    @property
    def end_time(self):
        return self._trigger_time + self._play_time


def process_midi_file(midi_file: mido.MidiFile):
    current_time = 0.0

    parsed_notes = []
    tempo = 0

    playing_notes = []

    for message in midi_file:
        if message.type == "set_tempo":
            tempo = message.tempo

        if message.type == "note_on":
            playing_notes.append(ParsedNote(
                value=message.note,
                velocity=message.velocity,
                channel=message.channel,
                trigger_time=current_time
            ))

        if message.type == "note_off" or (message.type == "note_on" and message.velocity == 0):
            for playing_note in playing_notes:
                if playing_note.value == message.note:
                    parsed_notes.append(ParsedNote(
                        value=playing_note.value,
                        velocity=playing_note.velocity,
                        channel=playing_note.channel,
                        trigger_time=playing_note.trigger_time,
                        play_time=current_time - playing_note.trigger_time
                    ))

                    playing_notes.remove(playing_note)

        current_time += message.time

    return tempo, parsed_notes


def main():
    window_width = 800
    window_height = 800

    midi_file = mido.MidiFile("jingle_bells.mid")
    tempo, parsed_notes = process_midi_file(midi_file)

    pygame.init()
    surface = pygame.display.set_mode([window_width, window_height], pygame.HWSURFACE)

    stop_event = threading.Event()
    t1 = threading.Thread(target=play_midi_file, args=[midi_file, stop_event])
    t1.start()

    is_running = True

    zoom = 300

    while is_running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                is_running = False

            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_q:
                    zoom = max(zoom - 15, 30)

                if event.key == pygame.K_e:
                    zoom = min(zoom + 15, 400)

        surface.fill((30, 30, 30))

        for parsed_note in parsed_notes:
            is_playing = parsed_note.trigger_time <= global_time <= parsed_note.trigger_time + parsed_note.play_time

            width = parsed_note.play_time * zoom
            height = 4

            x = window_width / 2 + (parsed_note.trigger_time - global_time) * zoom
            y = (window_height / 127) * parsed_note.value

            pygame.draw.rect(
                surface=surface,
                color=COLORS[-1] if is_playing else COLORS[parsed_note.channel % (len(COLORS) - 1)],
                rect=pygame.Rect(x, y, width, height),
                border_top_left_radius=2,
                border_top_right_radius=2,
                border_bottom_left_radius=2,
                border_bottom_right_radius=2
            )

        pygame.draw.rect(surface=surface, color=(0, 255, 0), rect=pygame.Rect(400, 0, 1, 800))

        pygame.display.flip()

    stop_event.set()
    pygame.quit()


if __name__ == "__main__":
    main()

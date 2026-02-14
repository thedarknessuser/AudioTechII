import numpy as np
from scipy.io.wavfile import read, write
from scipy import signal

# TODO: Replace the code below with your implementation of the waveforms.
# Hint: You may want to write more helper functions to create the waveforms
# Note: How will you handle aliasing?
def gen_wave(type, freq, dur, fs=44100, amp=1, phi=0):
    """
    Args:
    type (str) = waveform type: 'sine', 'square', 'saw', or 'triangle'
    freq (float) = fundamental frequency in Hz
    dur (float) = duration of the sinusoid (in seconds)
    fs (float) = sampling frequency of the sinusoid in Hz
    amp (float) = amplitude of the fundamental
    phi (float) = initial phase of the wave in radians
    Returns:
    The function should return a numpy array
    wave (numpy array) = The generated waveform
    """
  
    n = np.arange(0,dur,1/fs)
    wave = np.zeros(len(n))

    if type == 'sine':
        # create sinusoid
        wave = np.sin(2*np.pi * freq * n + phi)
    elif type == 'saw':
        for i in range(0, 100, 1):
            ithHarmonic = 1 + i
            ithFreq = freq * ithHarmonic
            if abs(ithFreq*2) >= fs:
                break
            ithWave = np.sin(2*np.pi * ithFreq * n + phi) * (1/ithHarmonic)
            wave += ithWave
    elif type == 'square':
        for i in range(0, 100, 1):
            ithHarmonic = 1 + 2*i
            ithFreq = freq * ithHarmonic
            if abs(ithFreq*2) >= fs:
                break
            ithWave = np.sin(2*np.pi * ithFreq * n + phi) * (1/ithHarmonic)
            wave += ithWave
    elif type == 'triangle':
        for i in range(0, 100, 1):
            ithHarmonic = 1 + 2*i
            ithFreq = freq * ithHarmonic
            if abs(ithFreq*2) >= fs:
                break
            ithWave = np.sin(2*np.pi * ithFreq * n + phi) * (1/(ithHarmonic**2))
            wave += ithWave
    return amp * wave
    

# TODO: Replace the code below with your implementation of an ADSR
# Hint: If you use %'s for your ADSR lengths, what length should the sustain value be
# Note: How will you handle percentages that are too long? For example, attack is 50, decay is 50, release is 50?
def adsr(data, attack, decay, sustain, release, fs=44100):
    """
    Args:
    data (np.array) = signal to be modified
    attack (float) = value between 0-100 representing what percentage of the note duration the attack should be
    decay (float) = value between 0-100 representing what percentage of the note duration the attack should be
    sustain (float) = value between 0-1 representing the amplitude of the sustain
    release (float) = value between 0-100 representing what percentage of the note duration the attack should be
    fs (float) = sampling frequency of the sinusoid in Hz
    Returns:
    The function should return a numpy array
    sig (numpy array) = the modified, enveloped signal
    """
    try: 
        attack + decay + sustain + release 
    except: 
        raise ValueError("ADSR parameters must be numeric.") 

    if sustain < 0 or sustain > 1: 
        raise ValueError("Sustain level must be between 0 and 1.") 

    N = len(data) 
    if N == 0: 
        return data 

    # scale if A + D + R exceed 100% 
    total = attack + decay + release 
    if total > 100: 
        scale = 100.0 / total 
        attack *= scale 
        decay *= scale 
        release *= scale 

    # convert percent → sample counts 
    a_n = int((attack/100.0) * N) 
    d_n = int((decay/100.0) * N) 
    r_n = int((release/100.0) * N) 
    s_n = N - (a_n + d_n + r_n) 

    # fix negative sustain length 
    if s_n < 0: 
        s_n = 0 
        r_n = max(0, r_n) 
        d_n = max(0, d_n) 
        a_n = max(0, a_n) 

    # build envelope 
    a = np.linspace(0, 1, a_n, endpoint=False) 
    d = np.linspace(1, sustain, d_n, endpoint=False) 
    s = np.full(s_n, sustain) 
    r = np.linspace(sustain, 0, r_n) 

    env = np.concatenate((a, d, s, r)) 

    # fix rounding mismatch 
    if len(env) != N: 
        env = env[:N] 
    return data * env


# TODO: Replace the code below with your implementation of a FM synthesis
# Hint: You should really be doing PM.
def fm_synth(carrier_type, carrier_freq, mod_index, mod_ratio, dur, fs=44100, amp=1, modulator_type='sine'):
    """
    Args:
    carrier_type (str) = carrier waveform type: 'sine', 'square', 'saw', or 'triangle'
    carrier_freq (float) = frequency of carrier in Hz
    mod_index (float) = index of modulation
    mod_ratio (float) = modulation ratio, where modulator frequency = carrier_freq * mod_ratio
    dur (float) = duration of the sinusoid (in seconds)
    fs (float) = sampling frequency of the sinusoid in Hz
    amp (float) = amplitude of the carrier
    modulator_type (str) = modulator waveform type: 'sine', 'square', 'saw', or 'triangle'

    Returns:
    The function should return a numpy array
    sig (numpy array) = frequency modulated signal
    """

    try: 
        carrier_freq + mod_index + mod_ratio + dur + fs + amp 
    except: 
        raise ValueError("carrier_freq, mod_index, mod_ratio, dur, fs, and amp must be numeric.") 

    if fs <= 0 or dur <= 0 or carrier_freq < 0: 
        return np.array([]) 

    # sanitize strings 
    try: 
        carrier = carrier_type.lower() 
    except:
        carrier = 'sine' 
    try: 
        modulator = modulator_type.lower() 
    except:
        modulator = 'sine' 

    n = np.arange(0,dur,1/fs)
    wave = np.zeros(len(n))
    mod_wave = gen_wave(modulator_type, carrier_freq * mod_ratio, dur, fs)

    if carrier_type == 'sine':
        # create sinusoid
        wave = np.sin(2*np.pi * carrier_freq * n + (mod_index * mod_wave))
    elif carrier_type == 'saw':
        for i in range(0, 100, 1):
            ithHarmonic = 1 + i
            ithFreq = carrier_freq * ithHarmonic
            if abs(ithFreq*2) >= fs:
                break
            ithWave = np.sin(2*np.pi * ithFreq * n + (mod_index * mod_wave)) * (1/ithHarmonic)
            wave += ithWave
    elif carrier_type == 'square':
        for i in range(0, 100, 1):
            ithHarmonic = 1 + 2*i
            ithFreq = carrier_freq * ithHarmonic
            if abs(ithFreq*2) >= fs:
                break
            ithWave = np.sin(2*np.pi * ithFreq * n + (mod_index * mod_wave)) * (1/ithHarmonic)
            wave += ithWave
    elif carrier_type == 'triangle':
        for i in range(0, 100, 1):
            ithHarmonic = 1 + 2*i
            ithFreq = carrier_freq * ithHarmonic
            if abs(ithFreq*2) >= fs:
                break
            ithWave = np.sin(2*np.pi * ithFreq * n + (mod_index * mod_wave)) * (1/(ithHarmonic**2))
            wave += ithWave
    return amp * wave

    

# TODO: Replace the code below with your implementation of a AM synthesis
def am_synth(carrier_type, carrier_freq, mod_depth, mod_ratio, dur, fs=44100, amp=1, modulator_type='sine'):
    """
    Args:
    carrier_type (str) = carrier waveform type: 'sine', 'square', 'saw', or 'triangle'
    carrier_freq (float) = frequency of carrier in Hz
    mod_depth (float) = depth of the modulator
    mod_ratio (float) = modulation ratio, where 1:mod_ratio is C:M
    dur (float) = duration of the sinusoid (in seconds)
    fs (float) = sampling frequency of the sinusoid in Hz
    amp (float) = amplitude of the carrier
    modulator_type (str) = modulator waveform type: 'sine', 'square', 'saw', or 'triangle'

    Returns:
    The function should return a numpy array
    sig (numpy array) = amplitude modulated signal
    """
    #sig = gen_wave(carrier_type, carrier_freq, dur, fs=fs)
    #return sig

    try: 
        carrier_freq + mod_depth + mod_ratio + dur + fs + amp 
    except: 
        raise ValueError("carrier_freq, mod_depth, mod_ratio, dur, fs, and amp must be numeric.") 

    if fs <= 0 or dur <= 0 or carrier_freq < 0: 
        return np.array([]) 

    # sanitize type strings 
    try: 
        carrier = carrier_type.lower() 
    except: 
        carrier = 'sine' 
    try: 
        modulator = modulator_type.lower() 
    except: 
        modulator = 'sine' 

    # clamp modulation depth to [0, 1] 
    if mod_depth < 0: 
        mod_depth = 0.0 
    if mod_depth > 1: 
        mod_depth = 1.0 

    # core params
    t = np.arange(0, dur, 1/fs)

    # modulator frequency and waveform (in [-1, 1]) 
    m = gen_wave(modulator_type, carrier_freq * mod_ratio, dur, fs)

    # amplitude envelope in [1 - mod_depth, 1] 
    #env = (1.0 - mod_depth) + mod_depth * (m + 1.0) * 0.5 

    # carrier waveform 
    c = gen_wave(carrier_type, carrier_freq, dur, fs)

    return amp * c * m 


# TODO: Complete at least one of the functions below: filter, reverb, delay.

# Note: I wrote this to only create low or highpass filters. You can alter to create bandpass/bandstop, but do not change the function definition.
def filter(data, type, cutoff_freq, fs=44100, order=5):
    """
    Args:
    data (np.array) = signal to be modified
    type (str) = filter type 'lowpass' or 'highpass'
    cutoff_freq (float) = cutoff frequency in Hz
    fs (float) = sampling frequency of the sinusoid in Hz
    order (int) = filter order

    Returns:
    The function should return a numpy array
    sig (numpy array) = filtered signal
    """
    sig = data
    return sig

def reverb(data, ir, dry_wet=0.5):
    """
    Args:
    data (np.array) = signal to be modified
    ir (str) = file path to impulse response
    dry_wet (float) = value between 0-1 dry/wet balance

    Returns:
    The function should return a numpy array
    sig (numpy array) = signal with reverb
    """
    #sig = data
    #return sig

    (fs, ir) = read(ir) 
    if ir.ndim > 1: 
        ir = ir.mean(axis=1)
    
    if data.ndim > 1: 
        data = data.mean(axis=1)
    
    print("reached convolve")
    wet_sig = np.convolve(data, ir, mode='full') 
    pad_length = len(wet_sig) - len(data) 
    dry_padded = np.concatenate([data, np.zeros(pad_length)]) 
    if np.max(np.abs(wet_sig)) > 0: #normalize 
        wet_norm = wet_sig / np.max(np.abs(wet_sig)) 
    else: 
        wet_norm = wet_sig 
    output = (dry_padded * (1.0 - dry_wet)) + (wet_norm * dry_wet) 

    return output

def delay(data, delay_time, dry_wet=0.5, fs=44100):
    """
    Args:
    data (np.array) = signal to be modified
    delay_time (float) = delay time in seconds
    dry_wet (float) = value between 0-1 dry/wet balance
    fs (float) = sampling frequency of the sinusoid in Hz

    Returns:
    The function should return a numpy array
    sig (numpy array) = signal with a delay
    """
    sig = data
    return sig

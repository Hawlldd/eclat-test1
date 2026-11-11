from PIL import Image
import numpy as np

ref_path = '/mnt/c/Users/raulr/.gemini/antigravity-acp/brain/f8e9a851-d8ad-45b0-8b50-6505fccf9681/.system_generated/logs/media/step_7922_media_0.png'
cur_path = '/mnt/c/Users/raulr/.gemini/antigravity-acp/brain/f8e9a851-d8ad-45b0-8b50-6505fccf9681/.system_generated/logs/media/step_8827_media_0.png'

ref = Image.open(ref_path).convert('RGB')
cur = Image.open(cur_path).convert('RGB')

print('Ref size:', ref.size)
print('Cur size:', cur.size)

# If scaled, resize to 240x160 using nearest neighbor
if ref.size != (240, 160):
    ref_240 = ref.resize((240, 160), Image.NEAREST)
else:
    ref_240 = ref

if cur.size != (240, 160):
    cur_240 = cur.resize((240, 160), Image.NEAREST)
else:
    cur_240 = cur

ref_240.save('/home/raulr/zEmerald/eclat-intro-teste/ref_240.png')
cur_240.save('/home/raulr/zEmerald/eclat-intro-teste/cur_240.png')
print('Saved ref_240.png and cur_240.png successfully')

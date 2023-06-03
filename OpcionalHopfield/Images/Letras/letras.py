from PIL import Image, ImageDraw, ImageFont
import os

absolute_path = os.path.dirname(__file__)
relative_path = "font.ttf"
font_path = os.path.join(absolute_path, relative_path)
# Define the font properties
font_size = 200

# Define the output directory
output_dir = os.path.join(absolute_path,"Imagenes", "")  # Replace with your desired output directory

# Create an image for each letter
for letter in range(65, 91):  # ASCII values for A to Z
    # Create a blank image with a white background
    image = Image.new("RGB", (font_size, font_size), (255, 255, 255))
    draw = ImageDraw.Draw(image)

    # Load the font and draw the letter in the center of the image
    font = ImageFont.truetype(font_path, font_size)
    letter_str = chr(letter)
    letter_width, letter_height = draw.textsize(letter_str, font=font)
    x = (font_size - letter_width) // 2
    y = (font_size - letter_height) // 2
    draw.text((x, y), letter_str, font=font, fill=(0, 0, 0))

    # Save the image as a PNG file
    image.save(output_dir + letter_str + ".png")

print("Images saved successfully.")

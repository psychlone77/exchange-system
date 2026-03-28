import csv
import random
import string

# Configuration
NUM_ROWS = (
    1_000_000  # Change this to 100 for a small test, or 10,000,000 for a stress test
)
INVALID_RATE = 0.05  # 5% of orders will be invalid
OUTPUT_FILE = "data/order_test.csv"

# Valid Data Pools
VALID_INSTRUMENTS = ["Rose", "Lavender", "Lotus", "Tulip", "Orchid"]
VALID_SIDES = [1, 2]


def generate_client_id():
    # Max 7 characters as per specifications
    return "".join(
        random.choices(string.ascii_letters + string.digits, k=random.randint(4, 7))
    )


def generate_valid_order():
    client_id = generate_client_id()
    instrument = random.choice(VALID_INSTRUMENTS)
    side = random.choice(VALID_SIDES)
    quantity = random.randint(1, 100) * 10
    price = round(random.uniform(1.0, 100.0), 2)

    return [client_id, instrument, side, quantity, f"{price:.2f}"]


def generate_invalid_order():
    # Start with a valid base
    order = generate_valid_order()

    # Randomly pick which validation rule to break
    error_type = random.randint(1, 5)

    if error_type == 1:
        # Missing Client Order ID
        order[0] = ""
    elif error_type == 2:
        # Invalid Instrument
        order[1] = random.choice(["Sunflower", "Daisy", "UnknownFlower"])
    elif error_type == 3:
        # Invalid Side
        order[2] = random.choice([0, 3, 99])
    elif error_type == 4:
        # Invalid Quantity (e.g., not a multiple of 10, or out of range)
        order[3] = random.choice([-10, 5, 105, 1005])
    elif error_type == 5:
        # Invalid Price (<= 0.0)
        order[4] = f"{round(random.uniform(-50.0, 0.0), 2):.2f}"

    return order


def main():
    print(f"Generating {NUM_ROWS} rows of test data...")
    print(f"Targeting ~{INVALID_RATE * 100}% invalid rows to trigger Validator checks.")

    # Open the file and start writing
    with open(OUTPUT_FILE, mode="w", newline="") as file:
        writer = csv.writer(file)

        # Write the exact header expected by our C++ CSV parser
        writer.writerow(["Cl. Ord.ID", "Instrument", "Side", "Quantity", "Price"])

        invalid_count = 0

        for i in range(NUM_ROWS):
            if random.random() < INVALID_RATE:
                writer.writerow(generate_invalid_order())
                invalid_count += 1
            else:
                writer.writerow(generate_valid_order())

            # Print progress for massive files
            if (i + 1) % 100_000 == 0:
                print(f"Generated {i + 1} rows...")

    print("-" * 40)
    print("Generation Complete!")
    print(f"Total Rows: {NUM_ROWS}")
    print(f"Valid Rows: {NUM_ROWS - invalid_count}")
    print(f"Invalid Rows: {invalid_count}")
    print(f"File saved to: {OUTPUT_FILE}")


if __name__ == "__main__":
    main()

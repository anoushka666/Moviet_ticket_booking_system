#include <stdio.h>
#include <string.h>
#include <stdlib.h>


struct Movie {
    int id;
    char name[50];
    char time[20];
    int seats[5][10];   
};

struct Booking {
    char customer[50];
    char movieName[50];
    int rowCol[50][2];
    int seatCount;
    char type[20];
};

struct Movie movies[3];
struct Booking bookings[100];
int bookingCount = 0;

void loadMovies() {
    
    movies[0].id = 1;
    strcpy(movies[0].name, "The Dark Knight");
    strcpy(movies[0].time, "12:30 PM");

    
    movies[1].id = 2;
    strcpy(movies[1].name, "Interstellar");
    strcpy(movies[1].time, "03:00 PM");

    
    movies[2].id = 3;
    strcpy(movies[2].name, "Avengers");
    strcpy(movies[2].time, "06:45 PM");

    
    for (int i = 0; i < 3; i++) {
        for (int r = 0; r < 5; r++) {
            for (int c = 0; c < 10; c++) {
                movies[i].seats[r][c] = 0;
            }
        }
    }
}

void showMovies() {
    printf("\n----- Available Movies -----\n");
    for (int i = 0; i < 3; i++) {
        printf("MOVIE ID: %d. %s (%s)\n", movies[i].id, movies[i].name, movies[i].time);
    }
}

void viewSeats(int movieId) {
    struct Movie m = movies[movieId - 1];

    printf("\nSeat Layout for %s (%s)\n", m.name, m.time);
    printf("0 = Available, X = Booked\n\n");

    printf("     --------------------  SCREEN\n");
    for (int i = 0; i < 5; i++) {
        printf("Row %d: ",i+1);
        for (int j = 0; j < 10; j++) {
            if (m.seats[i][j] == 1){
                printf("X ");
            }
            else
            printf("0 ");
        }
        printf("\n");
    }
}
void bookTicket() {
    int id;
    char name[50];

    showMovies();
    printf("\nEnter Movie ID to book ticket: ");
    scanf("%d", &id);

    if (id < 1 || id > 3) {
        printf("Invalid Movie ID!\n");
        return;
    }

    viewSeats(id);

    printf("\nEnter your first name: ");
    scanf("%s", name);

    struct Booking b;
    strcpy(b.customer, name);
    strcpy(b.movieName, movies[id - 1].name);

    printf("\nHow many seats do you want to book? ");
    scanf("%d", &b.seatCount);

    if (b.seatCount <= 0) {
        printf("Invalid number of seats!\n");
        return;
    }

    int priceClassic = 150;
    int pricePlatinum = 250;

    printf("\nChoose Seat Category:\n");
    printf("1. Classic (150)\n");
    printf("2. Platinum (250)\n");
    printf("Enter choice: ");
    int type;
    scanf("%d", &type);

    int price;
    if (type == 1) {
        strcpy(b.type, "Classic");
        price = priceClassic;
    } else {
        strcpy(b.type, "Platinum");
        price = pricePlatinum;
    }

    printf("\nSelect Seats (Row 0-4, Col 0-9)\n");

    for (int i = 0; i < b.seatCount; i++) {
        int r, c;
    seatPick:
        printf("Seat %d - Row Col: ", i + 1);
        scanf("%d %d", &r, &c);

        if (r < 0 || r >= 5 || c < 0 || c >= 10) {
            printf("Invalid seat! Try again.\n");
            goto seatPick;
        }
        if (movies[id - 1].seats[r][c] == 1) {
            printf("Seat already booked! Choose another.\n");
            goto seatPick;
        }

        movies[id - 1].seats[r][c] = 1;
        b.rowCol[i][0] = r;
        b.rowCol[i][1] = c;
    }

    bookings[bookingCount++] = b;

    printf("\n----- TICKET -----\n");
    printf("Name: %s\n", b.customer);
    printf("Movie: %s\n", b.movieName);
    printf("Seat Type: %s\n", b.type);
    printf("Seats: ");
    for (int i = 0; i < b.seatCount; i++)
        printf("[%d,%d] ", b.rowCol[i][0], b.rowCol[i][1]);

    int total = b.seatCount * price;
    printf("\nTotal Amount: %d\n", total);
}

   

void cancelTicket() {
    char name[50];
    printf("\nEnter your name to cancel booking: ");
    scanf("%s", name);

    int indices[100];  
    int count = 0;

    for (int i = 0; i < bookingCount; i++) {
        if (strcmp(bookings[i].customer, name) == 0) {
            indices[count++] = i;
        }
    }

    if (count == 0) {
        printf("\nNo bookings found for %s!\n", name);
        return;
    }

    printf("\n----- Your Booked Seats -----\n");
   for (int i = 0; i < count; i++) {
        int idx = indices[i];
        printf("%d. %s | Seats: ", i + 1, bookings[idx].movieName);

        for (int s = 0; s < bookings[idx].seatCount; s++)
            printf("[%d,%d] ", bookings[idx].rowCol[s][0], bookings[idx].rowCol[s][1]);
        printf("\n");
    }

    printf("\nCancel ALL seats? (1=Yes, 0=No): ");
    int all;
    scanf("%d", &all);

    if (all == 1) {
        int idx = indices[0];

        for (int s = 0; s < bookings[idx].seatCount; s++) {
            int r = bookings[idx].rowCol[s][0];
            int c = bookings[idx].rowCol[s][1];

            for (int m = 0; m < 3; m++) {
                if (strcmp(movies[m].name, bookings[idx].movieName) == 0)
                    movies[m].seats[r][c] = 0;
            }
        }

        for (int k = idx; k < bookingCount - 1; k++)
            bookings[k] = bookings[k + 1];
        bookingCount--;

        printf("\nAll seats cancelled!\n");
        return;
    }

    printf("\nHow many seats do you want to cancel? ");
    int n;
    scanf("%d", &n);

    if (n <= 0) return;

    int idx = indices[0];

    for (int i = 0; i < n; i++) {
        int r, c;
        printf("Enter seat to cancel (Row Col): ");
        scanf("%d %d", &r, &c);

        int removed = 0;
        for (int s = 0; s < bookings[idx].seatCount; s++) {
            if (bookings[idx].rowCol[s][0] == r && bookings[idx].rowCol[s][1] == c) {

                for (int m = 0; m < 3; m++)
                    if (strcmp(movies[m].name, bookings[idx].movieName) == 0)
                        movies[m].seats[r][c] = 0;

                for (int t = s; t < bookings[idx].seatCount - 1; t++)
                    bookings[idx].rowCol[t][0] = bookings[idx].rowCol[t + 1][0],
                    bookings[idx].rowCol[t][1] = bookings[idx].rowCol[t + 1][1];

                bookings[idx].seatCount--;
                removed = 1;
                break;
            }
        }
        if (!removed)
            printf("This seat is not in your booking!\n");
    }

    if (bookings[idx].seatCount == 0) {
        for (int k = idx; k < bookingCount - 1; k++)
            bookings[k] = bookings[k + 1];
        bookingCount--;
    }

    printf("\nCancellation updated!\n");
}
void showHistory() {
    printf("\n----- BOOKING HISTORY -----\n");

    if (bookingCount == 0) {
        printf("No bookings yet!\n");
        return;
    }

    for (int i = 0; i < bookingCount; i++) {
        printf("%d. %s | %s | Seats: ",
               i + 1,
               bookings[i].customer,
               bookings[i].movieName);

        for (int s = 0; s < bookings[i].seatCount; s++)
            printf("[%d,%d] ", bookings[i].rowCol[s][0], bookings[i].rowCol[s][1]);
        printf("\n");
    }
}



int main() {
    int choice;
    int id;
    loadMovies();

   while (1) {
    printf("\n============================================\n");
    printf("            Welcome to CinemaChai\n");
    printf("============================================\n\n");

    printf(" [1] Show Movies Now Playing\n");
    printf(" [2] Book Tickets\n");
    printf(" [3] View Seat Layout\n");
    printf(" [4] Cancel Your Ticket(s)\n");
    printf(" [5] Booking History\n");
    printf(" [6] Exit\n");

    printf("\n============================================\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            showMovies();
            break;

        case 2:
            bookTicket();
            break;

        case 3:
            printf("Enter Movie ID: ");
            scanf("%d", &id);
            if (id >= 1 && id <= 3)
                viewSeats(id);
            else
            printf("Invalid Movie ID!\n");
            break;

        case 4:
            cancelTicket();
            break;

        case 5:
            showHistory();
            break;

        case 6:
            printf("\nThank you for visiting CinemaChai!\n");
            return 0;

        default:
            printf("Invalid choice! Please try again.\n");
    }
}

}
   
#include <stdio.h>
#include <stdint.h>

// Bai tap: Truong huu han GF(2^8) chuan AES
// Da thuc toi gian m(x) = x^8 + x^4 + x^3 + x + 1 (Nhi phan: 0b100011011)
// Tap phan tu sinh {0, g^0, ..., g^N} voi N = 2^8 - 2 = 254

uint8_t exp8[255]; // Mang Antilog chua 255 đa thuc (tu g^0 den g^254)
uint8_t log8[256]; // Mang Log tra so mu (max index la 255)

// Ham khoi tao 2 bang tra cuu
void init_GF2_8_tables() {
    uint8_t g = 1; // Bat dau voi g^0 = 1
    
    // Lap sinh du cac phan tu luy thua tu g^0 den g^254
    for (int i = 0; i <= 254; i++) {
        exp8[i] = g;
        log8[g] = i;
        
        // Trong GF(2^8), phan tu sinh g = x + 1 (so 3)
        // Nhan da thuc voi (x + 1) nghia la: val * (x + 1) = val*x + val
        
        // Buoc 1: Tinh val*x bang cach dich trai 1 bit
        // Dung kieu 16 bit de giu lai bit bi tran truoc khi ha bac
        uint16_t g_times_x = g << 1; 
        
        // Buoc 2: Kiem tra tran x^8 (0b10000000 la x^7)
        if (g & 0b10000000) { 
            // Neu tran thi ha bac bang cach XOR voi m(x)
            g_times_x ^= 0b100011011; 
        }
        
        // Buoc 3: Hoan thanh phep nhan g*(x+1) = (g*x) + g
        // Phep cong trong truong GF(2) la phep XOR
        g = g_times_x ^ g; 
    }
}

// Ham nhan 2 da thuc f(x) va g(x)
uint8_t multiply_GF2_8(uint8_t fx, uint8_t gx) {
    if (fx == 0 || gx == 0) return 0;
    
    // Dung uint16_t de chua tong (vi 254 + 254 = 508 > 255)
    // Sau do modulo luon cho 255 (chu ky luy thua cua GF(2^8))
    uint16_t sum_log = (log8[fx] + log8[gx]) % 255;
    
    // Tra bang antilog de lay lai gia tri đa thuc
    return exp8[sum_log];
}

int main() {
    init_GF2_8_tables();

    // f(x) = x^5 + x^2 + x 
    // g(x) = x^7 + x^4 + x^3 + x^2 + x 
    uint8_t fx = 0b00100110; 
    uint8_t gx = 0b10011110; 
    
    // In ra f(x) va g(x) truoc khi nhan
    printf("f(x) = 0x%X\n", fx);
    printf("g(x) = 0x%X\n", gx);
    
    printf("Ket qua phep nhan GF(2^8): 0x%X\n", multiply_GF2_8(fx, gx)); 

    return 0;
}
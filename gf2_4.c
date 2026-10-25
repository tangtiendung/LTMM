#include <stdio.h>
#include <stdint.h>

// Bai tap: Truong huu han GF(2^4) 
// Da thuc toi gian m(x) = x^4 + x + 1 (Nhi phan: 0b10011)
// Tap phan tu sinh {0, g^0, ..., g^N} voi N = 2^4 - 2 = 14

uint8_t exp4[15]; // Mang Antilog chua gia tri đa thuc tu g^0 den g^14
uint8_t log4[16]; // Mang Log dung de tra so mu, co 16 o vi đa thuc max la 15 (0b1111)

// Ham khoi tao 2 bang tra cuu
void init_GF2_4_tables() {
    uint8_t g = 1; // Bat dau voi g^0 = 1
    
    // Lap de sinh du 15 phan tu luy thua tu g^0 den g^14
    for (int i = 0; i <= 14; i++) {
        exp4[i] = g;
        log4[g] = i;
        
        // Kiem tra xem da thuc hien tai co x^3 khong (0b1000)
        // Neu co thi lat nua dich bit x^3 se thanh x^4 bi tran bac
        uint8_t overflow = g & 0b1000; 
        
        // Nhan voi g (tuong duong nhan voi x) bang cach dich bit sang trai
        g <<= 1; 
        
        // Neu bi tran x^4 thi thuc hien ha bac (modulo cho da thuc toi gian m(x))
        if (overflow) {
            g ^= 0b10011; 
        }
    }
}

// Ham nhan 2 da thuc f(x) va g(x)
uint8_t multiply_GF2_4(uint8_t fx, uint8_t gx) {
    // Truong hop 1 trong 2 da thuc bang 0 thi ket qua nhan la 0
    if (fx == 0 || gx == 0) return 0; 
    
    // Tra bang log de lay so mu, roi cong 2 so mu lai
    // Chia lay du cho 15 vi day tu g^0 den g^14 co 15 phan tu quay vong
    uint8_t sum_log = (log4[fx] + log4[gx]) % 15; 
    
    // Tra bang antilog de lay lai gia tri đa thuc
    return exp4[sum_log]; 
}

int main() {
    init_GF2_4_tables();

    // f(x) = g^3 + g + 1 
    // g(x) = g^2 + g     
    uint8_t fx = 0b1011; 
    uint8_t gx = 0b0110; 
    
    // In ra f(x) va g(x) truoc khi nhan
    printf("f(x) = 0x%X\n", fx);
    printf("g(x) = 0x%X\n", gx);
    
    printf("Ket qua phep nhan GF(2^4): 0x%X\n", multiply_GF2_4(fx, gx)); 

    return 0;
}
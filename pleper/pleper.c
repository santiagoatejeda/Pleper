#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/sys.h"
#include "lwip/tcp.h"
#include "lcd.h"
#include <stdio.h>
#include <string.h>

#define GPIO_SDA 6
#define GPIO_SCL 7
#define ADC_WEB 26   // ADC0 → web
#define ADC_LCD 27   // ADC1 → LCD
#define ADC_MAX_VAL 4095.0f
#define VREF 3.3f

float R1 = 22000.0f;
float R2 = 10000.0f;

/* ===========================================================
   VARIABLES GLOBALES para sincronizar LCD + Web
   =========================================================== */
float valor_mostrado = 0.0f;
float pico_actual = 0.0f;
bool congelado = false;
absolute_time_t tiempo_limite;

/* =========================================================== */

float leer_voltaje(uint adc_num) {
    adc_select_input(adc_num);    
    uint16_t raw = adc_read();
    float v_adc = (raw / ADC_MAX_VAL) * VREF;
    return v_adc * ((R1 + R2) / R2);
}

// ------------------ Datos del día ------------------
float picos_dia[1000];
int cantidad_picos = 0;

// ------------------ HTTP: enviar ------------------
static err_t http_send(struct tcp_pcb *tpcb, const char *s) {
    err_t err = tcp_write(tpcb, s, strlen(s), TCP_WRITE_FLAG_COPY);
    if (err == ERR_OK) tcp_output(tpcb);
    return err;
}

// ------------------ Callback HTTP ------------------
static err_t recv_cb(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (!p) { tcp_close(tpcb); return ERR_OK; }
    char *req = p->payload;

    // -------- /adc --------
    if (strstr(req, "GET /adc")) {

        float v = valor_mostrado;

        if (v > 1.0f && cantidad_picos < 1000) {
            picos_dia[cantidad_picos++] = v;
        }

        char body[32];
        snprintf(body, sizeof(body), "%.2f", v);

        char header[128];
        snprintf(header, sizeof(header),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n\r\n%s",
        (int)strlen(body), body);

        http_send(tpcb, header);
    }

    // -------- /estado --------
    else if (strstr(req, "GET /estado")) {

        const char *estado_str = congelado ? "frozen" : "live";

        char header[128];
        snprintf(header, sizeof(header),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n\r\n%s",
        (int)strlen(estado_str), estado_str);

        http_send(tpcb, header);
    }

    // -------- /reset --------
    else if (strstr(req, "GET /reset")) {
        cantidad_picos = 0;

        const char r[] =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Connection: close\r\n\r\nOK";

        http_send(tpcb, r);
    }

    // -------- /picos --------
    else if (strstr(req, "GET /picos")) {

        char buf[5000];
        buf[0] = 0;

        strcat(buf, "[");

        for (int i = 0; i < cantidad_picos; i++) {
            char tmp[32];
            snprintf(tmp, sizeof(tmp), "%.2f", picos_dia[i]);
            strcat(buf, tmp);
            if (i < cantidad_picos - 1) strcat(buf, ",");
        }

        strcat(buf, "]");

        char header[512];
        snprintf(header, sizeof(header),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n\r\n%s",
        (int)strlen(buf), buf);

        http_send(tpcb, header);
    }

    // -------- Página principal --------
    else {

const char html[] =
"HTTP/1.1 200 OK\r\n"
"Content-Type: text/html\r\n"
"Connection: close\r\n\r\n"
"<!DOCTYPE html><html><head>"
"<meta charset='UTF-8'>"
"<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
"<title>PLEPER</title>"
"<style>"
"body{background:#111;color:#0f0;font-family:Arial;text-align:center;margin:0;padding:0;}"
"#valor{font-size:12vw;margin-top:4vw;transition:0.2s;}"
"#graf{width:94vw;height:50vw;border:1px solid #0f0;margin:3vw auto;display:block;}"
"@media(min-width:800px){#valor{font-size:60px;}#graf{width:800px;height:350px;}}"
"</style>"
"</head><body>"

"<h1 style='font-size:7vw;margin-top:4vw;'>Voltaje PLEPER</h1>"
"<div id='valor'>---</div>"
"<svg id='graf'></svg>"

"<script>"
"let datos=[];"
"let estado='live';"

"function actualizar(){"
" fetch('/adc').then(r=>r.text()).then(t=>{"
"   let v=parseFloat(t);"
"   document.getElementById('valor').innerText=t+' V';"
"   datos.push(v); if(datos.length>120)datos.shift();"
"   dibujar();"
" });"

" fetch('/estado').then(r=>r.text()).then(s=>{"
"   estado=s.trim();"
"   let el=document.getElementById('valor');"
"   if(estado==='frozen'){"
"       el.style.color='#00ff88';"
"       el.style.textShadow='0 0 15px #00ff88';"
"   } else {"
"       el.style.color='#0f0';"
"       el.style.textShadow='none';"
"   }"
" });"
"}"

"function dibujar(){"
" let svg=document.getElementById('graf');"
" let w=svg.clientWidth, h=svg.clientHeight;"
" svg.setAttribute('viewBox','0 0 '+w+' '+h);"
" svg.innerHTML='';"
" if(datos.length<2)return;"

" let max=Math.max(...datos), min=Math.min(...datos);"
" let pts='';"
" for(let i=0;i<datos.length;i++){"
"   let x=i*(w/(datos.length-1));"
"   let y=h-((datos[i]-min)/(max-min+0.001))*h;"
"   pts+=x+','+y+' ';"
" }"

" let p=document.createElementNS('http://www.w3.org/2000/svg','polyline');"
" p.setAttribute('points',pts);"
" p.setAttribute('fill','none');"
" p.setAttribute('stroke','#0f0');"
" p.setAttribute('stroke-width','3');"
" svg.appendChild(p);"

" for(let i=0;i<datos.length;i++){"
"   if(datos[i]>1.0){"
"     let x=i*(w/(datos.length-1));"
"     let y=h-((datos[i]-min)/(max-min+0.001))*h;"
"     let c=document.createElementNS('http://www.w3.org/2000/svg','circle');"
"     c.setAttribute('cx',x); c.setAttribute('cy',y);"
"     c.setAttribute('r','5'); c.setAttribute('fill','#00ff88');"
"     svg.appendChild(c);"
"   }"
" }"
"}"

"setInterval(actualizar,500);"
"</script>"

"</body></html>";

        http_send(tpcb, html);
    }

    pbuf_free(p);
    return ERR_OK;
}

static err_t accept_cb(void *arg, struct tcp_pcb *newpcb, err_t err) {
    LWIP_UNUSED_ARG(arg);
    LWIP_UNUSED_ARG(err);
    tcp_recv(newpcb, recv_cb);
    return ERR_OK;
}

// ---------- Iniciar servidor ----------
void iniciar_servidor(void) {
    struct tcp_pcb *pcb = tcp_new();
    tcp_bind(pcb, IP_ADDR_ANY, 80);
    pcb = tcp_listen(pcb);
    tcp_accept(pcb, accept_cb);
}

// ---------- Conexión WiFi ----------
bool conectar_wifi(void) {
    printf("Conectando a WiFi...\n");
    int r = cyw43_arch_wifi_connect_timeout_ms(
        "PLEPER", "pleper2025",
        CYW43_AUTH_WPA2_AES_PSK, 20000);
    return (r == 0);
}

// =======================================================
// ===============    PROGRAMA PRINCIPAL    ===============
// =======================================================
int main() {
    stdio_init_all();
    stdio_usb_init();

    i2c_init(i2c1, 100000);
    gpio_set_function(GPIO_SDA, GPIO_FUNC_I2C);
    gpio_set_function(GPIO_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(GPIO_SDA);
    gpio_pull_up(GPIO_SCL);
    lcd_init(i2c1, 0x27);

    adc_init();
    adc_gpio_init(ADC_WEB); // ADC0
    adc_gpio_init(ADC_LCD); // GP27 → ADC1

    if (cyw43_arch_init()) {
        lcd_clear();
        lcd_string("Error WiFi Init");
        return 1;
    }
    cyw43_arch_enable_sta_mode();

    lcd_clear();
    lcd_string("Conectando WiFi...");
    if (!conectar_wifi()) {
        lcd_clear();
        lcd_string("Error WiFi Conn");
        while (true) sleep_ms(1000);
    }

    // Obtener IP
    struct netif *n = netif_default;
    const ip4_addr_t *ip = netif_ip4_addr(n);

    for (int i=0; i<30; i++) {
        if (ip4_addr1(ip) != 0) break;
        sleep_ms(1000);
        ip = netif_ip4_addr(n);
    }

    char ipbuf[20];
    snprintf(ipbuf, sizeof(ipbuf), "%d.%d.%d.%d",
        ip4_addr1(ip), ip4_addr2(ip), ip4_addr3(ip), ip4_addr4(ip));

    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_string("Piezoelectrico:");
    lcd_set_cursor(1, 0);
    lcd_string(ipbuf);
    printf("Servidor HTTP activo en: http://%s\n", ipbuf);

    iniciar_servidor();

    // =======================================================
    // ===============   FREEZE ACUMULATIVO   ================
    // =======================================================

    absolute_time_t ultimo = get_absolute_time();

    while (true) {
        cyw43_arch_poll();

        if (absolute_time_diff_us(ultimo, get_absolute_time()) > 500000) {
            ultimo = get_absolute_time();

            float v_lcd = leer_voltaje(0);   // <<< ahora lee ADC0 correctamente

            v_lcd *= 3.0f;

            if (!congelado) {
                valor_mostrado = v_lcd;
            }

            if (v_lcd > 1.0f) {
                if (!congelado) {
                    congelado = true;
                    pico_actual = v_lcd;
                    valor_mostrado = v_lcd;
                }
                else if (v_lcd > pico_actual) {
                    pico_actual = v_lcd;
                    valor_mostrado = v_lcd;
                }

                tiempo_limite = make_timeout_time_ms(5000);
            }

            if (congelado && time_reached(tiempo_limite)) {
                congelado = false;
            }

            // Mostrar en LCD
            lcd_set_cursor(1, 0);
            lcd_string("                ");
            lcd_set_cursor(1, 0);

            char buf[17];
            snprintf(buf, sizeof(buf), "V= %.2fV", valor_mostrado);
            lcd_string(buf);
        }

        sleep_ms(20);
    }
}










































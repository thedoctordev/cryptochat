/**
 * JS
 */

// Declaração de variáveis
let menu = false;

function open_menu() { 
    let menu_element = document.getElementById("options");
    if (menu) {
        menu_element.className = "close-menu";
        setTimeout(()=> {menu_element.style.display = "none"}, 100);
    } else {
        menu_element.className = "open-menu";
        menu_element.style.display = "block";
    }
    menu = !menu;
}